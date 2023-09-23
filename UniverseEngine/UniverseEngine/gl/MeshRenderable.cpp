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

        glGenVertexArrays(1, &this->vao);

        glBindVertexArray(this->vao);
        {
            GlDebugNames::Set(GL_VERTEX_ARRAY, this->vao, Format("%s_VAO", mesh.name.c_str()));

            size_t vertexSize = sizeof(Vertex) * mesh.vertices.size();
            size_t indexSize = sizeof(uint32_t) * mesh.indices.size();

            this->vertexBuffer = std::make_shared<Buffer>(
                Format("%s_VERTEX_BUFFER", mesh.name.c_str()), device, physicalDevice,
                BufferUsageBits::VERTEX_BUFFER,
                static_cast<uint64_t>(vertexSize), BufferLocation::GPU_ONLY);
            this->indexBuffer = std::make_shared<Buffer>(
                Format("%s_INDEX_BUFFER", mesh.name.c_str()), device, physicalDevice,
                BufferUsageBits::INDEX_BUFFER,
                static_cast<uint64_t>(indexSize), BufferLocation::GPU_ONLY);

            void* data = this->vertexBuffer->Map();
            memcpy(data, mesh.vertices.data(), vertexSize);
            this->vertexBuffer->Unmap();

            data = this->indexBuffer->Map();
            memcpy(data, mesh.indices.data(), indexSize);
            this->indexBuffer->Unmap();

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  (void*)offsetof(Vertex, texCoord));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  (void*)offsetof(Vertex, color));
        }
        glBindVertexArray(0);
    }

    MeshRenderable::~MeshRenderable() {
        glDeleteVertexArrays(1, &this->vao);
    }

    void MeshRenderable::Draw(CmdList& cmdList) {
        glBindVertexArray(this->vao);
        cmdList.DrawElements(this->indexCount);
        glBindVertexArray(0);
    }
}  // namespace UniverseEngine
#endif