#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../CmdList.h"

#include <GL/glew.h>

#include "../Logging.h"
#include "../GraphicsPipeline.h"

namespace UniverseEngine {
    CmdList::CmdList(std::shared_ptr<LogicalDevice> device, const CmdQueue& cmdQueue) : device(device) {
    }

    CmdList::~CmdList() {
    }

    void CmdList::Begin() {
    }

    void CmdList::End() {
    }

    void CmdList::Reset() {
        this->graphicsPipeline.reset();
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

    void CmdList::BeginRenderPass(std::shared_ptr<RenderPass> renderPass,
                                  const Swapchain& swapchain) {
    }

    void CmdList::EndRenderPass() {
    }

    void CmdList::BindGraphicsPipeline(std::shared_ptr<GraphicsPipeline> graphicsPipeline) {
        glUseProgram(graphicsPipeline->ShaderProgram());

        // TODO: pipeline states like blending, depth etc.
        glEnable(GL_DEPTH_TEST);

        this->boundGraphicsPipeline = graphicsPipeline;
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

    void CmdList::PushConstant(const std::string& name, void* constant, size_t size) {
        UE_ASSERT_MSG(this->boundGraphicsPipeline, "No graphics pipeline bound.");

        unsigned shaderProgram = this->boundGraphicsPipeline->ShaderProgram();
        unsigned blockIndex = glGetUniformBlockIndex(shaderProgram, name.c_str());
        UE_ASSERT_MSG(blockIndex != GL_INVALID_INDEX, "Cannot find push constant '%s'.",
                      name.c_str());
        glUniformBlockBinding(shaderProgram, blockIndex, 0);

        unsigned ubo;
        glGenBuffers(1, &ubo);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, size, constant, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, size);

        // TODO: fix memory leak
        // glDeleteBuffers(1, &ubo);
    }
}  // namespace UniverseEngine
#endif