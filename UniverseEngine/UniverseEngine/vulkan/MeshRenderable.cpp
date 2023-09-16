#include "../CmdList.h"
#include "../Format.h"
#include "../Logging.h"
#include "../Mesh.h"

namespace UniverseEngine {
    MeshRenderable::MeshRenderable(const Mesh& mesh)
        : indexCount(static_cast<uint32_t>(mesh.indices.size())) {
        UE_ASSERT(mesh.vertices.size() > 0);
        UE_ASSERT(mesh.indices.size() > 0);
    }

    MeshRenderable::~MeshRenderable() {
    }

    void MeshRenderable::Draw(CmdList& cmdList) {
    }
}  // namespace UniverseEngine