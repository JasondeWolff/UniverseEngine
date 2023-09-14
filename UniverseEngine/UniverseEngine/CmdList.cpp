#include "CmdList.h"

#include <GL/glew.h>

#include "Logging.h"

namespace UniverseEngine {
    CmdList::CmdList() {
    }

    CmdList::~CmdList() {
    }

    void CmdList::Clear(const glm::vec4& clearColor) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void CmdList::SetScissor(const Rect2D& rect2D) {
        glScissor(static_cast<GLint>(rect2D.offset.x), static_cast<GLint>(rect2D.offset.y),
                  static_cast<GLsizei>(rect2D.extent.x), static_cast<GLsizei>(rect2D.extent.y));
    }

    void CmdList::SetViewport(const Rect2D& rect2D) {
        glViewport(static_cast<GLint>(rect2D.offset.x), static_cast<GLint>(rect2D.offset.y),
                  static_cast<GLsizei>(rect2D.extent.x), static_cast<GLsizei>(rect2D.extent.y));
    }

    void CmdList::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,
                       uint32_t firstInstance) {
        UE_ASSERT_MSG(instanceCount == 1, "GL doesn't support instancing.");
        UE_ASSERT_MSG(firstInstance == 0, "GL doesn't support instancing.");

        glDrawArrays(GL_TRIANGLES, static_cast<GLint>(firstVertex),
                     static_cast<GLsizei>(vertexCount));
    }

    void CmdList::DrawElements(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                               uint32_t firstInstance) {
        UE_ASSERT_MSG(instanceCount == 1, "GL doesn't support instancing.");
        UE_ASSERT_MSG(firstIndex == 0, "GL doesn't support index offsets.");
        UE_ASSERT_MSG(firstInstance == 0, "GL doesn't support instancing.");

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
    }
}  // namespace UniverseEngine