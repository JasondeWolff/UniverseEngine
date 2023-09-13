#pragma once

#include <optional>
#include <string>

#include "ShaderRenderable.h"

namespace UniverseEngine {
    class Graphics;

    struct Shader {
        std::string name;
        std::string sourceCode;

    private:
        friend class Graphics;
        std::optional<ShaderRenderable> renderable;

        void ClearCPUData();
    };
}