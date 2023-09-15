#include <GL/glew.h>

#include "CmdList.h"
#include "DebugNames.h"
#include "Format.h"
#include "Logging.h"
#include "Mesh.h"

namespace UniverseEngine {
    MeshRenderable::MeshRenderable(const Mesh& mesh)
        : indexCount(static_cast<uint32_t>(mesh.indices.size())) {
        UE_ASSERT(mesh.vertices.size() > 0);
        UE_ASSERT(mesh.indices.size() > 0);

        glGenVertexArrays(1, &this->vao);
        glGenBuffers(1, &this->vbo);
        glGenBuffers(1, &this->ebo);

        glBindVertexArray(this->vao);
        {
            DebugNames::Set(GL_VERTEX_ARRAY, this->vao, Format("%s_VAO", mesh.name.c_str()));

            glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
            DebugNames::Set(GL_BUFFER, this->vbo, Format("%s_VBO", mesh.name.c_str()));
            glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex),
                         mesh.vertices.data(),
                         GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
            DebugNames::Set(GL_BUFFER, this->ebo, Format("%s_EBO", mesh.name.c_str()));
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t),
                         mesh.indices.data(), GL_STATIC_DRAW);

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
        glDeleteBuffers(1, &this->vbo);
        glDeleteBuffers(1, &this->ebo);
        glDeleteVertexArrays(1, &this->vao);
    }

    void MeshRenderable::Draw(CmdList& cmdList) {
        glBindVertexArray(this->vao);
        cmdList.DrawElements(this->indexCount);
        glBindVertexArray(0);
    }
}  // namespace UniverseEngine