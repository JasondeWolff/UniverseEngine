#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Image.h"

namespace UniverseEngine {
    Image::Image(std::shared_ptr<LogicalDevice> device, uint32_t width, uint32_t height,
                 GraphicsFormat format) {
    
    }

    Image::~Image() {

    }
}
#endif