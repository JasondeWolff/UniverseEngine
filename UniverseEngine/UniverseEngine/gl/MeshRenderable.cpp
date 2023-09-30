#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include <GL/glew.h>

#include "../CmdList.h"
#include "../Format.h"
#include "../Logging.h"
#include "../Mesh.h"
#include "GlDebugNames.h"

namespace UniverseEngine {
    MeshRenderable::MeshRenderable(std::shared_ptr<LogicalDevice> device,
                                   const PhysicalDevice& physicalDevice,
                                   CmdList& uploadCmdList, const Mesh& mesh)
        : indexCount(static_cast<uint32_t>(mesh.indices.size())) {
        UE_ASSERT(mesh.vertices.size() > 0);
        UE_ASSERT(mesh.indices.size() > 0);

        {
            size_t vertexSize = sizeof(Vertex) * mesh.vertices.size();
            size_t indexSize = sizeof(uint32_t) * mesh.indices.size();

            this->vertexBuffer = std::make_shared<Buffer>(
                Format("%s_VERTEX_BUFFER", mesh.name.c_str()), device, physicalDevice,
                BufferUsageBits::VERTEX_BUFFER,
                static_cast<uint64_t>(vertexSize), BufferLocation::CPU_TO_GPU);
            this->indexBuffer = std::make_shared<Buffer>(
                Format("%s_INDEX_BUFFER", mesh.name.c_str()), device, physicalDevice,
                BufferUsageBits::INDEX_BUFFER, static_cast<uint64_t>(indexSize),
                BufferLocation::CPU_TO_GPU);

            void* data = this->vertexBuffer->Map();
            memcpy(data, mesh.vertices.data(), vertexSize);
            this->vertexBuffer->Unmap();

            data = this->indexBuffer->Map();
            memcpy(data, mesh.indices.data(), indexSize);
            this->indexBuffer->Unmap();
        }
    }

    MeshRenderable::~MeshRenderable() {
    }

    void MeshRenderable::Draw(CmdList& cmdList) {
        cmdList.BindVertexBuffer(this->vertexBuffer);
        cmdList.BindIndexBuffer(this->indexBuffer);
        cmdList.DrawElements(this->indexCount);
        glBindVertexArray(0);
    }
}  // namespace UniverseEngine
#endif