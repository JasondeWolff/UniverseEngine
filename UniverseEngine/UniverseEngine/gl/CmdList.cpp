#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../CmdList.h"

#include <GL/glew.h>

#include "../Logging.h"
#include "../GraphicsPipeline.h"
#include "../DescriptorSet.h"
#include "../Buffer.h"

namespace UniverseEngine {
    CmdList::CmdList(std::shared_ptr<LogicalDevice> device, const CmdQueue& cmdQueue) : device(device), cmdQueue(cmdQueue) {
    }

    CmdList::~CmdList() {
    }

    void CmdList::Begin() {
    }

    void CmdList::End() {
    }

    void CmdList::Reset() {
        for (auto pc : this->trackedPushConstants) {
            glDeleteBuffers(1, &pc);
        }
        this->trackedPushConstants.clear();

        this->boundGraphicsPipeline.reset();
        this->trackedRenderPasses.clear();
        this->trackedBuffers.clear();
        this->trackedDescriptorSets.clear();
    }

    void CmdList::TransitionImageLayout(std::shared_ptr<Image> image, ImageLayout oldLayout,
                                        ImageLayout newLayout) {
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
                                  const Framebuffer& framebuffer, const glm::vec4& clearColor) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void CmdList::EndRenderPass() {
    }

    void CmdList::BindDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet, uint32_t set) {
        for (auto& bufferBinding : descriptorSet->Buffers()) {
            uint32_t binding = bufferBinding.first;
            auto& buffer = bufferBinding.second;
            glBindBufferRange(buffer->GetIdentifier(), binding, buffer->GetBuffer(), 0, buffer->Size());
        }
       
        for (auto& imageBinding : descriptorSet->Images()) {
            uint32_t binding = imageBinding.first;
            auto& image = imageBinding.second.first;
            auto& sampler = imageBinding.second.second;

            glActiveTexture(GL_TEXTURE0 + binding);
            glBindTexture(GL_TEXTURE_2D, image->GetTexture());
        }

        trackedDescriptorSets.push_back(descriptorSet);
    }

    void CmdList::BindGraphicsPipeline(std::shared_ptr<GraphicsPipeline> graphicsPipeline) {
        glUseProgram(graphicsPipeline->ShaderProgram());

        // TODO: set pipeline states like blending, depth etc.
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

    void CmdList::PushConstant(const std::string& name, void* constant, size_t size, GraphicsStageFlags stageFlags) {
        UE_ASSERT_MSG(this->boundGraphicsPipeline, "No graphics pipeline bound.");

        unsigned ubo;
        glGenBuffers(1, &ubo);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, size, constant, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, 1, ubo, 0, size);

        this->trackedPushConstants.push_back(ubo);
    }
}  // namespace UniverseEngine
#endif