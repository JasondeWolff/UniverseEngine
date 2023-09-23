#pragma once

#include "Buffer.h"
#include "GraphicsAPI.h"

namespace UniverseEngine {
    class Graphics;
    struct Texture;
    class CmdList;
    class LogicalDevice;
    class PhysicalDevice;
    class Image;

    class TextureRenderable {
    public:
        ~TextureRenderable();

    private:
        friend class Graphics;
        TextureRenderable(std::shared_ptr<LogicalDevice> device,
                          const PhysicalDevice& physicalDevice,
                       CmdList& uploadCmdList, const Texture& texture);

#ifdef GRAPHICS_API_GL
        
#elif defined(GRAPHICS_API_VULKAN)
        std::shared_ptr<Image> image;
#endif
    };
}  // namespace UniverseEngine