#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "GraphicsAPI.h"
#include "Rect2D.h"

namespace UniverseEngine {
    class CmdList {
    public:
        CmdList();
        ~CmdList();

        CmdList(const CmdList& other) = delete;
        CmdList& operator=(const CmdList& other) = delete;

        void Clear(const glm::vec4& clearColor);

        void SetScissor(const Rect2D& rect2D);
        void SetViewport(const Rect2D& rect2D);

        void Draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0,
                  uint32_t firstInstance = 0);
        void DrawElements(uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0,
                          uint32_t firstIndex = 0);
    };
}  // namespace UniverseEngine