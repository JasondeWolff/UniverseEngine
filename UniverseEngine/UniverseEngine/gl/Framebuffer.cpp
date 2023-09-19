#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Framebuffer.h"
#include "../Image.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../RenderPass.h"

namespace UniverseEngine {
    Framebuffer::Framebuffer(std::shared_ptr<LogicalDevice> device, std::shared_ptr<Image> image,
                             std::shared_ptr<RenderPass> renderPass)
        : device(device), image(image), renderPass(renderPass) {
    }

    Framebuffer::Framebuffer(Framebuffer&& other) noexcept
        : device(other.device),
          image(other.image),
          renderPass(other.renderPass) {
    }

    Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
        return *this;
    }

    Framebuffer::~Framebuffer() {
    }
}  // namespace UniverseEngine
#endif