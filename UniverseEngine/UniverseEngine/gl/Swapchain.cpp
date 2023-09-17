#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Swapchain.h"

namespace UniverseEngine {
    Swapchain::Swapchain(const Window& window, const GraphicsInstance& instance,
                         const LogicalDevice& device,
                         const PhysicalDevice& physicalDevice) {
    }

    Swapchain::~Swapchain() {
    }
}  // namespace UniverseEngine
#endif