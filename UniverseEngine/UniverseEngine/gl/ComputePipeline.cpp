#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../ComputePipeline.h"
#include "../DescriptorSetLayout.h"
#include "../Format.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../Mesh.h"
#include "../PushConstantRange.h"
#include "../RenderPass.h"
#include "../ShaderRenderable.h"
#include "GlDebugNames.h"

namespace UniverseEngine {
    ComputePipeline::ComputePipeline(
        std::shared_ptr<LogicalDevice> device, const ShaderRenderable& shader,
        std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts)
        : device(device), descriptorSetLayouts(descriptorSetLayouts) {
        this->shaderProgram = glCreateProgram();

        std::string debugName =
            Format("- %s\n", GlDebugNames::Get(GL_SHADER, shader.GetShader()).c_str());
        debugName = Format("[\n%s]", debugName.c_str());
        GlDebugNames::Set(GL_PROGRAM, this->shaderProgram, debugName);

        glAttachShader(this->shaderProgram, shader.GetShader());
        glLinkProgram(this->shaderProgram);

        GLint success;
        GLchar log[1024 * 4];
        glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(this->shaderProgram, sizeof(log), NULL, log);
            UE_FATAL("Failed to compile program '%s'.\n---------------------------------\n%s",
                     debugName.c_str(), log);
        }
    }

    ComputePipeline::~ComputePipeline() {
        glDeleteProgram(this->shaderProgram);
    }

    unsigned ComputePipeline::ShaderProgram() const {
        return this->shaderProgram;
    }
}  // namespace UniverseEngine
#endif