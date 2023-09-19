#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Swapchain.h"

namespace UniverseEngine {
    Swapchain::Swapchain(const Window& window, const GraphicsInstance& instance,
                         std::shared_ptr<LogicalDevice> device,
                         const PhysicalDevice& physicalDevice)
        : device(device) {
        this->width = window.Width();
        this->height = window.Height();
    }

    Swapchain::~Swapchain() {
    }

    const Framebuffer& Swapchain::GetCurrentFramebuffer() {
        return *this->framebuffer;
    }

    void Swapchain::RebuildFramebuffers(std::shared_ptr<RenderPass> renderPass) {
        this->framebuffer.reset();
        this->framebuffer = std::move(std::make_unique<Framebuffer>(
            this->device,
            std::make_shared<Image>(this->device, this->width, this->height,
                                    GraphicsFormat::R8G8B8A8_SRGB),
            renderPass));
    }

    GraphicsFormat Swapchain::Format() const {
        return GraphicsFormat::UNDEFINED;
    }

    Rect2D Swapchain::Extent() const {
        return Rect2D(this->width, this->height);
    }
}  // namespace UniverseEngine
#endif