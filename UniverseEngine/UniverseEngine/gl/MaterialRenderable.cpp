#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../Material.h"
#include "../MaterialRenderable.h"
#include "../PhysicalDevice.h"

namespace UniverseEngine {
    MaterialRenderable::MaterialRenderable(std::shared_ptr<LogicalDevice> device,
                                           const PhysicalDevice& physicalDevice,
                                           const Material& material) {
    }

    MaterialRenderable::~MaterialRenderable() {
    }
}  // namespace UniverseEngine
#endif