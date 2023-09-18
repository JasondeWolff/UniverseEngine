#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Swapchain.h"

namespace UniverseEngine {
    Swapchain::Swapchain(const Window& window, const GraphicsInstance& instance,
                         std::shared_ptr<LogicalDevice> device,
                         const PhysicalDevice& physicalDevice) {
        this->width = window.Width();
        this->height = window.Height();
    }

    Swapchain::~Swapchain() {
    }

    void Swapchain::RebuildFramebuffers(const RenderPass& renderPass) {
    }

    GraphicsFormat Swapchain::Format() const {
        return GraphicsFormat::UNDEFINED;
    }

    Rect2D Swapchain::Extent() const {
        return Rect2D(this->width, this->height);
    }
}  // namespace UniverseEngine
#endif