#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>
#include <vector>

#include "GraphicsAPI.h"
#include "Rect2D.h"

namespace UniverseEngine {
    class GraphicsPipeline;
    class CmdQueue;
    class LogicalDevice;
    class Swapchain;
    class RenderPass;
    class Framebuffer;
    class Buffer;

    class CmdList {
    public:
        ~CmdList();
        CmdList(const CmdList& other) = delete;
        CmdList& operator=(const CmdList& other) = delete;

        void Begin();
        void End();
        void Reset();

        void CopyBuffers(std::shared_ptr<Buffer> src, std::shared_ptr<Buffer> dst);

        void SetScissor(const Rect2D& rect2D);
        void SetViewport(const Rect2D& rect2D);

        void BindVertexBuffer(std::shared_ptr<Buffer> vertexBuffer);
        void BindIndexBuffer(std::shared_ptr<Buffer> indexBuffer);

        // TODO: Replace swapchain with a framebuffer abstraction
        void BeginRenderPass(std::shared_ptr<RenderPass> renderPass, const Framebuffer& framebuffer,
                             const glm::vec4& clearColor);
        void EndRenderPass();

        void BindGraphicsPipeline(std::shared_ptr<GraphicsPipeline> graphicsPipeline);

        void Draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0,
                  uint32_t firstInstance = 0);
        void DrawElements(uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstIndex = 0,
                          uint32_t firstInstance = 0);

        template <typename T>
        void PushConstant(const std::string& name, T& constant) {
            this->PushConstant(name, static_cast<void*>(&constant), sizeof(T));
        }

    private:
        friend class CmdQueue;
        CmdList(std::shared_ptr<LogicalDevice> device, const CmdQueue& cmdQueue);

        const std::shared_ptr<LogicalDevice> device;
        const CmdQueue& cmdQueue;

#ifdef GRAPHICS_API_GL
#elif defined(GRAPHICS_API_VULKAN)
        VkCommandBuffer cmdBuffer;
#endif

        void PushConstant(const std::string& name, void* constant, size_t size);

        std::shared_ptr<GraphicsPipeline> boundGraphicsPipeline;
        std::vector<std::shared_ptr<RenderPass>> trackedRenderPasses;
        std::vector<std::shared_ptr<Buffer>> trackedBuffers;
    };
}  // namespace UniverseEngine