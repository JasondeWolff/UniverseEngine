#pragma once

#include <array>
#include <memory>

#include "GraphicsAPI.h"
#include "Swapchain.h"

namespace UniverseEngine {
    class Graphics;
    struct Material;
    class CmdList;
    class LogicalDevice;
    class PhysicalDevice;
    struct DescriptorLayoutBinding;
    class DescriptorPool;
    class DescriptorSetLayout;
    class DescriptorSet;
    class Buffer;
    class Sampler;

    class MaterialRenderable {
    public:
        ~MaterialRenderable();

        void Update(size_t currentFrame, std::shared_ptr<Sampler> sampler, CmdList& cmdList);
        void Bind(CmdList& cmdList, size_t currentFrame, uint32_t set);

        static const std::shared_ptr<DescriptorSetLayout>& DescriptorLayout(
            std::shared_ptr<LogicalDevice> device);

    private:
        friend class Graphics;
        MaterialRenderable(std::shared_ptr<LogicalDevice> device,
                           const PhysicalDevice& physicalDevice,
                           std::shared_ptr<DescriptorPool> descriptorPool,
                           const Material& material);

        const std::shared_ptr<LogicalDevice> device;
        const PhysicalDevice& physicalDevice;
        const Material& material;

#ifdef GRAPHICS_API_GL
#elif defined(GRAPHICS_API_VULKAN)
        std::array<std::shared_ptr<Buffer>, Swapchain::MAX_FRAMES_IN_FLIGHT> uniformBuffers;
        std::array<std::shared_ptr<DescriptorSet>, Swapchain::MAX_FRAMES_IN_FLIGHT> descriptorSets;
#endif
    };
}  // namespace UniverseEngine