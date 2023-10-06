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
        size_t imageSize = texture.width * texture.height * texture.depth * 4;

        std::shared_ptr<Buffer> stagingBuffer =
            std::make_shared<Buffer>(Format("%s_STAGING_IMAGE_BUFFER", texture.name.c_str()),
                                     device, physicalDevice, BufferUsageBits::TRANSFER_SRC_BUFFER,
                                     static_cast<uint64_t>(imageSize), BufferLocation::CPU_TO_GPU);

        void* data = stagingBuffer->Map();
        memcpy(data, texture.data, imageSize);
        stagingBuffer->Unmap();

        GraphicsFormat format = (texture.type == TextureType::SRGB)
                                    ? GraphicsFormat::R8G8B8A8_SRGB
                                    : GraphicsFormat::R8G8B8A8_UNORM;

        this->image = std::make_shared<Image>(
            texture.name, device, physicalDevice, texture.width, texture.height, texture.mips,
            ImageUsageBits::TRANSFER_SRC_IMAGE | ImageUsageBits::TRANSFER_DST_IMAGE |
                ImageUsageBits::SAMPLED_IMAGE,
            format, 1, texture.dimensions, texture.depth);

        uploadCmdList.TransitionImageLayout(this->image, ImageLayout::TRANSFER_DST_OPTIMAL,
                                            ResourceAccessBits::ACCESS_TRANSFER_WRITE_BIT,
                                            PipelineStageBits::PIPELINE_STAGE_TRANSFER_BIT);
        uploadCmdList.CopyBuffers(stagingBuffer, this->image);
        if (texture.mips > 1) {
            uploadCmdList.GenerateMips(this->image);
        } else {
            uploadCmdList.TransitionImageLayout(
                this->image, ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                ResourceAccessBits::ACCESS_SHADER_READ_BIT,
                PipelineStageBits::PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
        }
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
        cmdList.TransitionImageLayout(image, ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                                      ResourceAccessBits::ACCESS_SHADER_READ_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
        return image;
    }
}  // namespace UniverseEngine
#endif