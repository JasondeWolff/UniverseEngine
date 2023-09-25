#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <array>

#include "../Buffer.h"
#include "../CmdList.h"
#include "../CmdQueue.h"
#include "../DescriptorSet.h"
#include "../GraphicsPipeline.h"
#include "../Logging.h"
#include "../Swapchain.h"
#include "VkConversion.h"

namespace UniverseEngine {
    VkImageLayout GetVkImageLayout(ImageLayout layout) {
        switch (layout) {
            case ImageLayout::UNDEFINED:
                return VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
            case ImageLayout::GENERAL:
                return VkImageLayout::VK_IMAGE_LAYOUT_GENERAL;
            case ImageLayout::COLOR_ATTACHMENT_OPTIMAL:
                return VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                return VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case ImageLayout::DEPTH_STENCIL_READ_ONLY_OPTIMAL:
                return VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            case ImageLayout::SHADER_READ_ONLY_OPTIMAL:
                return VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case ImageLayout::TRANSFER_SRC_OPTIMAL:
                return VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case ImageLayout::TRANSFER_DST_OPTIMAL:
                return VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        }

        UE_FATAL("Unsupported image layout");
        return VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
    }

    CmdList::CmdList(std::shared_ptr<LogicalDevice> device, const CmdQueue& cmdQueue)
        : device(device), cmdQueue(cmdQueue) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = cmdQueue.GetCmdPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        UE_ASSERT_MSG(!vkAllocateCommandBuffers(device->GetDevice(), &allocInfo, &this->cmdBuffer),
                      "Failed to allocate command buffer.");
    }

    CmdList::~CmdList() {
        vkFreeCommandBuffers(this->device->GetDevice(), this->cmdQueue.GetCmdPool(), 1,
                             &this->cmdBuffer);
    }

    void CmdList::Begin() {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        UE_ASSERT_MSG(!vkBeginCommandBuffer(this->cmdBuffer, &beginInfo),
                      "Failed to begin command buffer.");
    }

    void CmdList::End() {
        UE_ASSERT_MSG(!vkEndCommandBuffer(this->cmdBuffer), "Failed to end command buffer.");
    }

    void CmdList::Reset() {
        vkResetCommandBuffer(this->cmdBuffer, 0);

        this->boundGraphicsPipeline.reset();
        this->trackedRenderPasses.clear();
        this->trackedBuffers.clear();
        this->trackedImages.clear();
        this->trackedDescriptorSets.clear();
    }

    void CmdList::CopyBuffers(std::shared_ptr<Buffer> src, std::shared_ptr<Buffer> dst) {
        UE_ASSERT_MSG(src->Size() == dst->Size(),
                      "Copy buffers src and dst must have the same size.");

        VkBufferCopy copyRegion{};
        copyRegion.size = src->Size();

        vkCmdCopyBuffer(this->cmdBuffer, src->GetBuffer(), dst->GetBuffer(), 1, &copyRegion);

        this->trackedBuffers.push_back(src);
        this->trackedBuffers.push_back(dst);
    }

    void CmdList::CopyBuffers(std::shared_ptr<Buffer> src, std::shared_ptr<Image> dst) {
        VkBufferImageCopy copyRegion{};
        copyRegion.bufferOffset = 0;
        copyRegion.bufferRowLength = 0;
        copyRegion.bufferImageHeight = 0;
        copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copyRegion.imageSubresource.mipLevel = 0;
        copyRegion.imageSubresource.baseArrayLayer = 0;
        copyRegion.imageSubresource.layerCount = 1;
        copyRegion.imageOffset = {0, 0, 0};
        copyRegion.imageExtent = {dst->Width(), dst->Height(), 1};

        vkCmdCopyBufferToImage(this->cmdBuffer, src->GetBuffer(), dst->GetImage(),
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

        this->trackedBuffers.push_back(src);
        this->trackedImages.push_back(dst);
    }

    void CmdList::TransitionImageLayout(std::shared_ptr<Image> image, ImageLayout oldLayout,
                                        ImageLayout newLayout) {
        VkImageLayout vkOldLayout = GetVkImageLayout(oldLayout);
        VkImageLayout vkNewLayout = GetVkImageLayout(newLayout);

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = vkOldLayout;
        barrier.newLayout = vkNewLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image->GetImage();
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (vkOldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
            vkNewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (vkOldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                   vkNewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            UE_FATAL("Unsupported image layout transition.");
        }

        vkCmdPipelineBarrier(this->cmdBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0,
                             nullptr, 1, &barrier);

        this->trackedImages.push_back(image);
    }

    void CmdList::SetScissor(const Rect2D& rect2D) {
        VkRect2D vkRect{};
        vkRect.offset.x = static_cast<uint32_t>(rect2D.offset.x);
        vkRect.offset.y = static_cast<uint32_t>(rect2D.offset.y);
        vkRect.extent.width = static_cast<uint32_t>(rect2D.extent.x);
        vkRect.extent.height = static_cast<uint32_t>(rect2D.extent.y);

        vkCmdSetScissor(this->cmdBuffer, 0, 1, &vkRect);
    }

    void CmdList::SetViewport(const Rect2D& rect2D) {
        VkViewport viewport{};
        viewport.width = static_cast<float>(rect2D.extent.x);
        viewport.height = -static_cast<float>(rect2D.extent.y);
        viewport.x = static_cast<float>(rect2D.offset.x);
        viewport.y = static_cast<float>(rect2D.offset.y) - viewport.height;
        viewport.maxDepth = 1.0;

        vkCmdSetViewport(this->cmdBuffer, 0, 1, &viewport);
    }

    void CmdList::BindVertexBuffer(std::shared_ptr<Buffer> vertexBuffer) {
        VkBuffer vertexBuffers[] = {vertexBuffer->GetBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(this->cmdBuffer, 0, 1, vertexBuffers, offsets);

        this->trackedBuffers.push_back(vertexBuffer);
    }

    void CmdList::BindIndexBuffer(std::shared_ptr<Buffer> indexBuffer) {
        vkCmdBindIndexBuffer(this->cmdBuffer, indexBuffer->GetBuffer(), 0,
                             VkIndexType::VK_INDEX_TYPE_UINT32);

        this->trackedBuffers.push_back(indexBuffer);
    }

    void CmdList::BeginRenderPass(std::shared_ptr<RenderPass> renderPass,
                                  const Framebuffer& framebuffer, const glm::vec4& clearColor) {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass->GetRenderPass();
        renderPassInfo.framebuffer = framebuffer.GetFramebuffer();
        renderPassInfo.renderArea.extent =
            VkExtent2D{framebuffer.Images()[0]->Width(), framebuffer.Images()[0]->Height()};

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {clearColor.r, clearColor.g, clearColor.b, clearColor.a};
        clearValues[1].depthStencil = {1.0f, 0};
        
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(this->cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        this->trackedRenderPasses.push_back(renderPass);
    }

    void CmdList::EndRenderPass() {
        vkCmdEndRenderPass(this->cmdBuffer);
    }

    void CmdList::BindDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet) {
        VkDescriptorSet descriptorSets[] = {descriptorSet->GetDescriptorSet()};
        vkCmdBindDescriptorSets(this->cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                this->boundGraphicsPipeline->GetLayout(), 0, 1, descriptorSets, 0,
                                nullptr);

        trackedDescriptorSets.push_back(descriptorSet);
    }

    void CmdList::BindGraphicsPipeline(std::shared_ptr<GraphicsPipeline> graphicsPipeline) {
        vkCmdBindPipeline(this->cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          graphicsPipeline->GetPipeline());

        this->boundGraphicsPipeline = graphicsPipeline;
    }

    void CmdList::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,
                       uint32_t firstInstance) {
        vkCmdDraw(this->cmdBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void CmdList::DrawElements(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                               uint32_t firstInstance) {
        vkCmdDrawIndexed(this->cmdBuffer, indexCount, instanceCount, firstIndex, 0, firstInstance);
    }

    void CmdList::PushConstant(const std::string& name, void* constant, size_t size,
                               GraphicsStageFlags stageFlags) {
        vkCmdPushConstants(this->cmdBuffer, this->boundGraphicsPipeline->GetLayout(),
                           GetVkShaderStageFlags(stageFlags), 0, size, constant);
    }
}  // namespace UniverseEngine
#endif