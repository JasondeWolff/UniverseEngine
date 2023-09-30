#include "Shader.h"

namespace UniverseEngine {
    const ShaderRenderable& Shader::Renderable() const {
        return *this->renderable;
    }

    void Shader::ClearCPUData() {
        this->sourceCode.clear();
        this->sourceCode.shrink_to_fit();
        this->source.clear();
        this->source.shrink_to_fit();
    }
}