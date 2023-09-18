#include "Shader.h"

namespace UniverseEngine {
    void Shader::ClearCPUData() {
        this->sourceCode.clear();
        this->sourceCode.shrink_to_fit();
        this->source.clear();
        this->source.shrink_to_fit();
    }
}