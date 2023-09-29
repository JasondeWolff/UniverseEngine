#include "../TextureRenderable.h"
#ifdef GRAPHICS_API_VULKAN

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
        size_t imageSize = texture.width * texture.height * 4;

        std::shared_ptr<Buffer> stagingBuffer =
            std::make_shared<Buffer>(Format("%s_STAGING_IMAGE_BUFFER", texture.name.c_str()),
                                     device, physicalDevice, BufferUsageBits::TRANSFER_SRC_BUFFER,
                                     static_cast<uint64_t>(imageSize), BufferLocation::CPU_TO_GPU);

        void* data = stagingBuffer->Map();
        memcpy(data, texture.data, imageSize);
        stagingBuffer->Unmap();

        this->image = std::make_shared<Image>(
            texture.name, device, physicalDevice, texture.width, texture.height, texture.mips,
            ImageUsageBits::TRANSFER_SRC_IMAGE | ImageUsageBits::TRANSFER_DST_IMAGE |
                ImageUsageBits::SAMPLED_IMAGE,
            GraphicsFormat::R8G8B8A8_SRGB);

        uploadCmdList.TransitionImageLayout(this->image, ImageLayout::UNDEFINED,
                                            ImageLayout::TRANSFER_DST_OPTIMAL);
        uploadCmdList.CopyBuffers(stagingBuffer, this->image);
        uploadCmdList.GenerateMips(this->image);
    }

    TextureRenderable::~TextureRenderable() {
    }

    const std::shared_ptr<Image>& TextureRenderable::EmptyImage(
        std::shared_ptr<LogicalDevice> device, const PhysicalDevice& physicalDevice,
        CmdList& cmdList) {
        static std::shared_ptr<Image> image = std::make_shared<Image>(
            "Empty", device, physicalDevice, 1, 1, 1,
            ImageUsageBits::TRANSFER_DST_IMAGE | ImageUsageBits::SAMPLED_IMAGE,
            GraphicsFormat::R8G8B8A8_SRGB);
        cmdList.TransitionImageLayout(image, ImageLayout::UNDEFINED,
                                      ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        return image;
    }
}  // namespace UniverseEngine
#endif