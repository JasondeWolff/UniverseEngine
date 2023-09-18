#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../RenderPass.h"
#include "VkGraphicsFormat.h"

namespace UniverseEngine {
    RenderPass::RenderPass(std::shared_ptr<LogicalDevice> device, GraphicsFormat colorFormat)
        : device(device) {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = VkGraphicsFormat::To(colorFormat);
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        UE_ASSERT_MSG(
            !vkCreateRenderPass(device->GetDevice(), &renderPassInfo, nullptr, &this->renderPass),
            "Failed to create render pass.");
    }

    RenderPass::~RenderPass() {
        vkDestroyRenderPass(this->device->GetDevice(), this->renderPass, nullptr);
    }

    VkRenderPass RenderPass::GetRenderPass() const {
        return this->renderPass;
    }
}  // namespace UniverseEngine
#endif