#pragma once

#include "GraphicsAPI.h"

#include "Buffer.h"

namespace UniverseEngine {
    class Graphics;
    struct Mesh;
    class CmdList;
    class LogicalDevice;
    class PhysicalDevice;
    class CmdList;

    class MeshRenderable {
    public:
        ~MeshRenderable();

    private:
        friend class Graphics;
        MeshRenderable(std::shared_ptr<LogicalDevice> device, const PhysicalDevice& physicalDevice, CmdList& uploadCmdList, const Mesh& mesh);

        void Draw(CmdList& cmdList);

        uint32_t indexCount;

#ifdef GRAPHICS_API_GL
        unsigned vao;
        unsigned vbo;
        unsigned ebo;
#elif defined(GRAPHICS_API_VULKAN)
        std::shared_ptr<Buffer> vertexBuffer;
#endif
    };
}  // namespace UniverseEngine