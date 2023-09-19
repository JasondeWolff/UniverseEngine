#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Framebuffer.h"
#include "../Image.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../RenderPass.h"

namespace UniverseEngine {
    Framebuffer::Framebuffer(std::shared_ptr<LogicalDevice> device, std::shared_ptr<Image> image,
                             std::shared_ptr<RenderPass> renderPass)
        : device(device), image(image), renderPass(renderPass) {
        VkImageView attachments[] = {image->GetImageView()};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->GetRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = image->Width();
        framebufferInfo.height = image->Height();
        framebufferInfo.layers = 1;

        UE_ASSERT_MSG(!vkCreateFramebuffer(this->device->GetDevice(), &framebufferInfo, nullptr,
                                           &this->framebuffer),
                      "Failed to create frame buffer.");
    }

    Framebuffer::Framebuffer(Framebuffer&& other) noexcept
        : device(other.device),
          image(other.image),
          renderPass(other.renderPass),
          framebuffer(other.framebuffer) {
        other.framebuffer = VK_NULL_HANDLE;
    }

    Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
        this->framebuffer = other.framebuffer;
        other.framebuffer = VK_NULL_HANDLE;
        return *this;
    }

    Framebuffer::~Framebuffer() {
        if (this->framebuffer) {
            vkDestroyFramebuffer(this->device->GetDevice(), this->framebuffer, nullptr);
        }
    }

    VkFramebuffer Framebuffer::GetFramebuffer() const {
        return this->framebuffer;
    }
}  // namespace UniverseEngine
#endif