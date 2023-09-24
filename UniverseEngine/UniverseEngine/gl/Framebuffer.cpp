#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Framebuffer.h"
#include "../Image.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../RenderPass.h"

namespace UniverseEngine {
    Framebuffer::Framebuffer(std::shared_ptr<LogicalDevice> device, std::vector<std::shared_ptr<Image>> images,
                             std::shared_ptr<RenderPass> renderPass)
        : device(device), images(images), renderPass(renderPass) {
    }

    Framebuffer::Framebuffer(Framebuffer&& other) noexcept
        : device(other.device),
          images(other.images),
          renderPass(other.renderPass) {
    }

    Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
        this->images = other.images;
        this->renderPass = other.renderPass;
        return *this;
    }

    Framebuffer::~Framebuffer() {
    }

    const std::vector<std::shared_ptr<Image>>& Framebuffer::Images() const {
        return this->images;
    }
}  // namespace UniverseEngine
#endif