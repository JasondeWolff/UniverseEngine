#ifdef GRAPHICS_API_GL
#include "Mesh.h"

#include "Logging.h"

namespace UniverseEngine {
    MeshRenderable::MeshRenderable(const Mesh& mesh) {
        UE_INFO("Builded renderable!");
    }
}  // namespace UniverseEngine

#endif