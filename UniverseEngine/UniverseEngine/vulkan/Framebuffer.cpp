#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Framebuffer.h"
#include "../Image.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../RenderPass.h"

namespace UniverseEngine {
    Framebuffer::Framebuffer(std::shared_ptr<LogicalDevice> device,
                             std::vector<std::shared_ptr<Image>> images,
                             std::shared_ptr<RenderPass> renderPass)
        : device(device), images(images), renderPass(renderPass) {
        UE_ASSERT_MSG(images.size() > 0, "Framebuffer requires at least 1 image.");

        std::vector<VkImageView> attachments;
        for (auto image : images) {
            attachments.push_back(image->GetImageView());
        }

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->GetRenderPass();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = images[0]->Width();
        framebufferInfo.height = images[0]->Height();
        framebufferInfo.layers = 1;

        UE_ASSERT_MSG(!vkCreateFramebuffer(this->device->GetDevice(), &framebufferInfo, nullptr,
                                           &this->framebuffer),
                      "Failed to create frame buffer.");
    }

    Framebuffer::Framebuffer(Framebuffer&& other) noexcept
        : device(other.device),
          images(other.images),
          renderPass(other.renderPass),
          framebuffer(other.framebuffer) {
        other.framebuffer = VK_NULL_HANDLE;
    }

    Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
        this->framebuffer = other.framebuffer;
        this->renderPass = other.renderPass;
        this->images = other.images;
        other.framebuffer = VK_NULL_HANDLE;
        return *this;
    }

    Framebuffer::~Framebuffer() {
        if (this->framebuffer) {
            vkDestroyFramebuffer(this->device->GetDevice(), this->framebuffer, nullptr);
        }
    }

    const std::vector<std::shared_ptr<Image>>& Framebuffer::Images() const {
        return this->images;
    }

    VkFramebuffer Framebuffer::GetFramebuffer() const {
        return this->framebuffer;
    }
}  // namespace UniverseEngine
#endif