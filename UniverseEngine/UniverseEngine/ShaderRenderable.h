#pragma once

#include "GraphicsAPI.h"
#include "ShaderType.h"

namespace UniverseEngine {
    class Graphics;
    struct Shader;
    class LogicalDevice;

    class ShaderRenderable {
    public:
        ~ShaderRenderable();

    private:
        friend class Graphics;
        ShaderRenderable(std::shared_ptr<LogicalDevice> device, const Shader& shader);

        const std::shared_ptr<LogicalDevice> device;
        ShaderType type;

#ifdef GRAPHICS_API_GL
    public:
        unsigned GetShader() const;

    private:
        unsigned shader;
#elif defined(GRAPHICS_API_VULKAN)
    public:
        VkShaderModule GetShader() const;

    private:
        VkShaderModule shader;
#endif
    };
}  // namespace UniverseEngine