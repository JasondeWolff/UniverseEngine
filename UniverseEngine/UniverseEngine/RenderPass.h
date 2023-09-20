#pragma once

#include <memory>
#include <vector>

#include "GraphicsAPI.h"
#include "GraphicsFormat.h"

namespace UniverseEngine {
    class LogicalDevice;

    class RenderPass {
    public:
        RenderPass(std::shared_ptr<LogicalDevice> device, GraphicsFormat colorFormat);
        ~RenderPass();
        RenderPass(const RenderPass& other) = delete;
        RenderPass& operator=(const RenderPass& other) = delete;

    private:
        const std::shared_ptr<LogicalDevice> device;

#ifdef GRAPHICS_API_GL
#elif defined(GRAPHICS_API_VULKAN)
    public:
        VkRenderPass GetRenderPass() const;

    private:
        VkRenderPass renderPass;
#endif
    };
}  // namespace UniverseEngine