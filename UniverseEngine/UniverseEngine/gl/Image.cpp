#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Image.h"

namespace UniverseEngine {
    Image::Image(const std::string& name, std::shared_ptr<LogicalDevice> device,
                 const PhysicalDevice& physicalDevice, uint32_t width, uint32_t height,
                 GraphicsFormat format) {
    
    }

    Image::~Image() {

    }
}
#endif