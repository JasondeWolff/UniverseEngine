#include "../ShaderRenderable.h"

#include "../Logging.h"
#include "../Shader.h"

namespace UniverseEngine {
    ShaderRenderable::ShaderRenderable(const Shader& shader) {
        UE_ASSERT_MSG(shader.sourceCode.size() > 0, "Cannot build renderable from empty shader.");
    }

    ShaderRenderable::~ShaderRenderable() {
    }
}  // namespace UniverseEngine