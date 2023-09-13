#include "GraphicsPipeline.h"

#include <GL/glew.h>

#include "DebugNames.h"
#include "Format.h"
#include "Logging.h"
#include "ShaderRenderable.h"

namespace UniverseEngine {
    GraphicsPipeline::GraphicsPipeline(const std::vector<ShaderRenderable*>& shaders) {
        this->shaderProgram = glCreateProgram();

        std::string debugName = "";
        for (auto shader : shaders) {
            debugName += Format("- %s\n", DebugNames::Get(GL_SHADER, shader->GetShader()).c_str());
        }
        debugName = Format("[\n%s]", debugName.c_str());
        DebugNames::Set(GL_PROGRAM, this->shaderProgram, debugName);

        for (auto shader : shaders) {
            glAttachShader(this->shaderProgram, shader->GetShader());
        }
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

    GraphicsPipeline::~GraphicsPipeline() {
        glDeleteProgram(this->shaderProgram);
    }

    unsigned GraphicsPipeline::ShaderProgram() const {
        return this->shaderProgram;
    }
}  // namespace UniverseEngine