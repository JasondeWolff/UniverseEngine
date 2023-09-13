#include "Mesh.h"

#ifdef GRAPHICS_API_GL
#include <GL/glew.h>

#include "Logging.h"

namespace UniverseEngine {
    MeshRenderable::MeshRenderable(const Mesh& mesh) {
        UE_ASSERT(mesh.vertices.size() > 0);
        UE_ASSERT(mesh.indices.size() > 0);

        glGenVertexArrays(1, &this->vao);
        glGenBuffers(1, &this->vbo);
        glGenBuffers(1, &this->ebo);

        glBindVertexArray(this->vao);
        {
            glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
            glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices,
                         GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t),
                         &mesh.indices, GL_STATIC_DRAW);

            //glEnableVertexAttribArray(0);

        }
    }

    void MeshRenderable::Draw() {
    }
}  // namespace UniverseEngine

#endif