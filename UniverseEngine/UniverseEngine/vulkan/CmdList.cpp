#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../CmdList.h"
#include "../CmdQueue.h"
#include "../GraphicsPipeline.h"
#include "../Logging.h"
#include "../Swapchain.h"
#include "../Buffer.h"
#include "../DescriptorSet.h"

namespace UniverseEngine {
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
        vkCmdBindIndexBuffer(this->cmdBuffer, indexBuffer->GetBuffer(), 0, VkIndexType::VK_INDEX_TYPE_UINT32);

        this->trackedBuffers.push_back(indexBuffer);
    }

    void CmdList::BeginRenderPass(std::shared_ptr<RenderPass> renderPass,
                                  const Framebuffer& framebuffer, const glm::vec4& clearColor) {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass->GetRenderPass();
        renderPassInfo.framebuffer = framebuffer.GetFramebuffer();
        renderPassInfo.renderArea.extent =
            VkExtent2D{framebuffer.image->Width(), framebuffer.image->Height()};

        VkClearValue vkClearColor = {{{clearColor.r, clearColor.g, clearColor.b, clearColor.a}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &vkClearColor;

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

    void CmdList::PushConstant(const std::string& name, void* constant, size_t size) {
    }
}  // namespace UniverseEngine
#endif