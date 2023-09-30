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

        std::shared_ptr<Image> GetImage() const;

        static const std::shared_ptr<Image>& EmptyImage(std::shared_ptr<LogicalDevice> device,
                                                        const PhysicalDevice& physicalDevice, CmdList& cmdList);

    private:
        friend class Graphics;
        TextureRenderable(std::shared_ptr<LogicalDevice> device,
                          const PhysicalDevice& physicalDevice,
                       CmdList& uploadCmdList, const Texture& texture);

        std::shared_ptr<Image> image;
    };
}  // namespace UniverseEngine