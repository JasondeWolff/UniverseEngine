#include "ShaderRenderable.h"

#include <GL/glew.h>

#include "DebugNames.h"
#include "Logging.h"
#include "Shader.h"

namespace UniverseEngine {
    GLenum GetShaderType(const Shader& shader) {
        switch (shader.type) {
            case ShaderType::VERTEX:
                return GL_VERTEX_SHADER;
            case ShaderType::FRAGMENT:
                return GL_FRAGMENT_SHADER;
            default: {
                UE_FATAL("Unsupported shader type.");
                return NULL;
            }
        }
    }

    ShaderRenderable::ShaderRenderable(const Shader& shader) {
        UE_ASSERT_MSG(shader.sourceCode.size() > 0, "Cannot build renderable from empty shader.");

        this->shader = glCreateShader(GetShaderType(shader));
        DebugNames::Set(GL_SHADER, this->shader, shader.name);

        const char* pSourceCode = shader.sourceCode.c_str();
        glShaderSource(this->shader, 1, &pSourceCode, NULL);
        glCompileShader(this->shader);

        GLint success;
        GLchar log[1024 * 4];
        glGetShaderiv(this->shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(this->shader, sizeof(log), NULL, log);
            UE_FATAL("Failed to compile shader '%s'.\n---------------------------------\n%s", shader.name.c_str(), log);
        }
    }

    ShaderRenderable::~ShaderRenderable() {
        glDeleteShader(this->shader);
    }

    unsigned ShaderRenderable::GetShader() const {
        return this->shader;
    }
}  // namespace UniverseEngine