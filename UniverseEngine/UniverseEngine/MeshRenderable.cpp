#include "Mesh.h"

#ifdef GRAPHICS_API_GL
#include <GL/glew.h>

#include "DebugNames.h"
#include "Format.h"
#include "Logging.h"

namespace UniverseEngine {
    MeshRenderable::MeshRenderable(const Mesh& mesh) : indexCount(mesh.indices.size()) {
        UE_ASSERT(mesh.vertices.size() > 0);
        UE_ASSERT(mesh.indices.size() > 0);

        glGenVertexArrays(1, &this->vao);
        glGenBuffers(1, &this->vbo);
        glGenBuffers(1, &this->ebo);

        DebugNames::Set(GL_VERTEX_ARRAY, this->vao, Format("%s_VAO", mesh.name.c_str()));
        DebugNames::Set(GL_BUFFER, this->vbo, Format("%s_VBO", mesh.name.c_str()));
        DebugNames::Set(GL_BUFFER, this->ebo, Format("%s_EBO", mesh.name.c_str()));

        glBindVertexArray(this->vao);
        {
            glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
            glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices,
                         GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t),
                         &mesh.indices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  (void*)offsetof(Vertex, normal));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  (void*)offsetof(Vertex, texCoord));
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  (void*)offsetof(Vertex, color));
        }
        glBindVertexArray(0);
    }

    void MeshRenderable::Draw() {
        glBindVertexArray(this->vao);
        {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indexCount), GL_UNSIGNED_INT,
                           0);
        }
        glBindVertexArray(0);
    }
}  // namespace UniverseEngine

#endif