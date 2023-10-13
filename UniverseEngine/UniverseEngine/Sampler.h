#pragma once

#include <memory>
#include <string>

#include "GraphicsAPI.h"
#include "GraphicsFormat.h"

namespace UniverseEngine {
    class LogicalDevice;
    class PhysicalDevice;

    enum class FilterMode {
        NEAREST,
        LINEAR
    };

    struct SamplerInfo {
        FilterMode filterMode = FilterMode::LINEAR;
    };

    class Sampler {
    public:
        Sampler(const std::string& name, std::shared_ptr<LogicalDevice> device,
                const PhysicalDevice& physicalDevice, const SamplerInfo& info);
        ~Sampler();
        Sampler(const Sampler& other) = delete;
        Sampler& operator=(const Sampler& other) = delete;

    private:
        const std::shared_ptr<LogicalDevice> device;
#ifdef GRAPHICS_API_GL

#elif defined(GRAPHICS_API_VULKAN)
    public:
        VkSampler GetSampler() const;

    private:
        VkSampler sampler;
#endif
    };
}  // namespace UniverseEngine