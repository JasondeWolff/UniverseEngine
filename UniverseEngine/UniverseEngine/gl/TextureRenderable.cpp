#include "../TextureRenderable.h"
#ifdef GRAPHICS_API_GL

#include "../CmdList.h"
#include "../Format.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../PhysicalDevice.h"
#include "../Texture.h"

namespace UniverseEngine {
    TextureRenderable::TextureRenderable(std::shared_ptr<LogicalDevice> device,
                                         const PhysicalDevice& physicalDevice,
                                         CmdList& uploadCmdList, const Texture& texture) {
        this->image = std::make_shared<Image>(texture.data, texture.width, texture.height,
                                              GraphicsFormat::R8G8B8A8_SRGB);
    }

    TextureRenderable::~TextureRenderable() {
    }

    const std::shared_ptr<Image>& TextureRenderable::EmptyImage(
        std::shared_ptr<LogicalDevice> device, const PhysicalDevice& physicalDevice,
        CmdList& cmdList) {
        static std::shared_ptr<Image> image = std::make_shared<Image>(
            "Empty", device, physicalDevice, 1, 1,
            ImageUsageBits::TRANSFER_DST_IMAGE | ImageUsageBits::SAMPLED_IMAGE,
            GraphicsFormat::R8G8B8A8_SRGB);
        cmdList.TransitionImageLayout(image, ImageLayout::UNDEFINED,
                                      ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        return image;
    }
}  // namespace UniverseEngine
#endif