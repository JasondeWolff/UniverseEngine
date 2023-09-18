#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../Shader.h"
#include "../ShaderRenderable.h"
#include "VkDebugNames.h"

namespace UniverseEngine {
    ShaderRenderable::ShaderRenderable(std::shared_ptr<LogicalDevice> device, const Shader& shader)
        : device(device), type(shader.type) {
        UE_ASSERT_MSG(shader.source.size() > 0, "Cannot build renderable from empty shader.");

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shader.source.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shader.source.data());

        UE_ASSERT_MSG(
            !vkCreateShaderModule(device->GetDevice(), &createInfo, nullptr, &this->shader),
            "Failed to create shader.");
        VkDebugNames::Set(*device, VkObjectType::VK_OBJECT_TYPE_SHADER_MODULE,
                          reinterpret_cast<uint64_t>(this->shader),
                          shader.name);
    }

    ShaderRenderable::~ShaderRenderable() {
        vkDestroyShaderModule(this->device->GetDevice(), this->shader, nullptr);
    }

    VkShaderModule ShaderRenderable::GetShader() const {
        return this->shader;
    }
}  // namespace UniverseEngine
#endif