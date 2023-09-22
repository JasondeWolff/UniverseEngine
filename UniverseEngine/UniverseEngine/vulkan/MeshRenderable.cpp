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
                                   const PhysicalDevice& physicalDevice, CmdList& uploadCmdList,
                                   const Mesh& mesh)
        : indexCount(static_cast<uint32_t>(mesh.indices.size())) {
        UE_ASSERT(mesh.vertices.size() > 0);
        UE_ASSERT(mesh.indices.size() > 0);

        size_t vertexSize = sizeof(Vertex) * mesh.vertices.size();
        size_t indexSize = sizeof(uint32_t) * mesh.indices.size();

        std::shared_ptr<Buffer> stagingVertexBuffer =
            std::make_shared<Buffer>(Format("%s_STAGING_VERTEX_BUFFER", mesh.name.c_str()), device,
                                     physicalDevice, BufferUsageBits::TRANSFER_SRC,
                                     static_cast<uint64_t>(vertexSize), BufferLocation::CPU_TO_GPU);
        std::shared_ptr<Buffer> stagingIndexBuffer =
            std::make_shared<Buffer>(Format("%s_STAGING_INDEX_BUFFER", mesh.name.c_str()), device,
                                     physicalDevice, BufferUsageBits::TRANSFER_SRC,
                                     static_cast<uint64_t>(indexSize), BufferLocation::CPU_TO_GPU);

        void* data = stagingVertexBuffer->Map();
        memcpy(data, mesh.vertices.data(), vertexSize);
        stagingVertexBuffer->Unmap();

        data = stagingIndexBuffer->Map();
        memcpy(data, mesh.indices.data(), indexSize);
        stagingIndexBuffer->Unmap();

        this->vertexBuffer = std::make_shared<Buffer>(
            Format("%s_VERTEX_BUFFER", mesh.name.c_str()), device, physicalDevice,
            BufferUsageBits::TRANSFER_DST | BufferUsageBits::VERTEX_BUFFER,
            static_cast<uint64_t>(vertexSize), BufferLocation::GPU_ONLY);
        this->indexBuffer = std::make_shared<Buffer>(
            Format("%s_INDEX_BUFFER", mesh.name.c_str()), device, physicalDevice,
            BufferUsageBits::TRANSFER_DST | BufferUsageBits::INDEX_BUFFER,
            static_cast<uint64_t>(indexSize), BufferLocation::GPU_ONLY);

        uploadCmdList.CopyBuffers(stagingVertexBuffer, this->vertexBuffer);
        uploadCmdList.CopyBuffers(stagingIndexBuffer, this->indexBuffer);
    }

    MeshRenderable::~MeshRenderable() {
    }

    void MeshRenderable::Draw(CmdList& cmdList) {
        cmdList.BindVertexBuffer(this->vertexBuffer);
        cmdList.BindIndexBuffer(this->indexBuffer);
        cmdList.Draw(this->indexCount);
    }
}  // namespace UniverseEngine
#endif