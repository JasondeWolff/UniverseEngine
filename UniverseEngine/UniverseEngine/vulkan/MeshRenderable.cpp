#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../CmdList.h"
#include "../Format.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../Mesh.h"
#include "../PhysicalDevice.h"

namespace UniverseEngine {
    MeshRenderable::MeshRenderable(std::shared_ptr<LogicalDevice> device,
                                   const PhysicalDevice& physicalDevice, const Mesh& mesh)
        : indexCount(static_cast<uint32_t>(mesh.vertices.size())) {
        UE_ASSERT(mesh.vertices.size() > 0);
        UE_ASSERT(mesh.indices.size() > 0);

        size_t size = sizeof(Vertex) * mesh.vertices.size();

        this->vertexBuffer = std::make_shared<Buffer>(
            Format("%s_VERTEX_BUFFER", mesh.name.c_str()), device, physicalDevice,
            BufferUsageBit::VERTEX_BUFFER, static_cast<uint64_t>(size), BufferLocation::GPU_ONLY);

        void* data = this->vertexBuffer->Map();
        memcpy(data, mesh.vertices.data(), size);
        this->vertexBuffer->Unmap();
    }

    MeshRenderable::~MeshRenderable() {
    }

    void MeshRenderable::Draw(CmdList& cmdList) {
        cmdList.BindVertexBuffer(this->vertexBuffer);
        cmdList.Draw(this->indexCount);
    }
}  // namespace UniverseEngine
#endif