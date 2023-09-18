#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ShaderRenderable.h"
#include "ShaderType.h"

namespace UniverseEngine {
    class Graphics;

    struct Shader {
        std::string name;
        ShaderType type;

        std::string sourceCode;
        std::vector<char> source;

    private:
        friend class Graphics;
        std::unique_ptr<ShaderRenderable> renderable;

        void ClearCPUData();
    };
}  // namespace UniverseEngine