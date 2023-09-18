#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>

#include "GraphicsAPI.h"
#include "Rect2D.h"

namespace UniverseEngine {
    class GraphicsPipeline;
    class CmdQueue;
    class LogicalDevice;

    class CmdList {
    public:
        ~CmdList();
        CmdList(const CmdList& other) = delete;
        CmdList& operator=(const CmdList& other) = delete;

        void Reset();

        void Clear(const glm::vec4& clearColor);

        void SetScissor(const Rect2D& rect2D);
        void SetViewport(const Rect2D& rect2D);

        void BindGraphicsPipeline(std::shared_ptr<GraphicsPipeline> graphicsPipeline);

        void Draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0,
                  uint32_t firstInstance = 0);
        void DrawElements(uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0,
                          uint32_t firstIndex = 0);

        template <typename T>
        void PushConstant(const std::string& name, T& constant) {
            this->PushConstant(name, static_cast<void*>(&constant), sizeof(T));
        }

        private:
        friend class CmdQueue;
            CmdList(std::shared_ptr<LogicalDevice> device, const CmdQueue& cmdQueue);
    
        const std::shared_ptr<LogicalDevice> device;

#ifdef GRAPHICS_API_GL
#elif defined(GRAPHICS_API_VULKAN)
        VkCommandBuffer cmdBuffer;
#endif

        void PushConstant(const std::string& name, void* constant, size_t size);

        std::shared_ptr<GraphicsPipeline> graphicsPipeline;
    };
}  // namespace UniverseEngine