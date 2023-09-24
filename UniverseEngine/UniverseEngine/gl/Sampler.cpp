#include "../Sampler.h"
#ifdef GRAPHICS_API_GL

#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../PhysicalDevice.h"
#include "GlDebugNames.h"

namespace UniverseEngine {
    Sampler::Sampler(const std::string& name, std::shared_ptr<LogicalDevice> device,
                     const PhysicalDevice& physicalDevice)
        : device(device) {
        
    }

    Sampler::~Sampler() {
        
    }
}  // namespace UniverseEngine
#endif