#pragma once

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>
#include <vector>

#include "GraphicsAPI.h"
#include "GraphicsPipeline.h"
#include "GraphicsStageFlags.h"
#include "Image.h"
#include "Rect2D.h"

namespace UniverseEngine {
    class CmdQueue;
    class LogicalDevice;
    class Swapchain;
    class RenderPass;
    class Framebuffer;
    class Buffer;
    class DescriptorSet;
    class ComputePipeline;

    typedef std::bitset<17> ResourceAccess;
    enum ResourceAccessBits {
        ACCESS_INDIRECT_COMMAND_READ_BIT = 0x00000001,
        ACCESS_INDEX_READ_BIT = 0x00000002,
        ACCESS_VERTEX_ATTRIBUTE_READ_BIT = 0x00000004,
        ACCESS_UNIFORM_READ_BIT = 0x00000008,
        ACCESS_INPUT_ATTACHMENT_READ_BIT = 0x00000010,
        ACCESS_SHADER_READ_BIT = 0x00000020,
        ACCESS_SHADER_WRITE_BIT = 0x00000040,
        ACCESS_COLOR_ATTACHMENT_READ_BIT = 0x00000080,
        ACCESS_COLOR_ATTACHMENT_WRITE_BIT = 0x00000100,
        ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT = 0x00000200,
        ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT = 0x00000400,
        ACCESS_TRANSFER_READ_BIT = 0x00000800,
        ACCESS_TRANSFER_WRITE_BIT = 0x00001000,
        ACCESS_HOST_READ_BIT = 0x00002000,
        ACCESS_HOST_WRITE_BIT = 0x00004000,
        ACCESS_MEMORY_READ_BIT = 0x00008000,
        ACCESS_MEMORY_WRITE_BIT = 0x00010000,
    };
    size_t GetResourceAccessBitIndex(ResourceAccessBits bits);

    class CmdList {
    public:
        ~CmdList();
        CmdList(const CmdList& other) = delete;
        CmdList& operator=(const CmdList& other) = delete;

        void Begin();
        void End();
        void Reset();

        void CopyBuffers(std::shared_ptr<Buffer> src, std::shared_ptr<Buffer> dst);
        void CopyBuffers(std::shared_ptr<Buffer> src, std::shared_ptr<Image> dst);
        void CopyImages(std::shared_ptr<Image> src, std::shared_ptr<Image> dst);
        void CopyImagesIntoCubemap(const std::array<std::shared_ptr<Image>, 6>& images,
                                   std::shared_ptr<Image> cubemap);
        void GenerateMips(std::shared_ptr<Image> image);

        void TransitionImageLayout(std::shared_ptr<Image> image, ImageLayout layout, ResourceAccess access, PipelineStage pipelineStage);

        void SetScissor(const Rect2D& rect2D);
        void SetViewport(const Rect2D& rect2D, bool normalize = true);

        void BindVertexBuffer(std::shared_ptr<Buffer> vertexBuffer);
        void BindIndexBuffer(std::shared_ptr<Buffer> indexBuffer);

        void BeginRenderPass(std::shared_ptr<RenderPass> renderPass, const Framebuffer& framebuffer,
                             const glm::vec4& clearColor);
        void EndRenderPass();

        void BindDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet, uint32_t set,
                               PipelineType pipelineType = PipelineType::GRAPHICS);

        void BindGraphicsPipeline(std::shared_ptr<GraphicsPipeline> graphicsPipeline);
        void BindComputePipeline(std::shared_ptr<ComputePipeline> computePipeline);

        void Draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0,
                  uint32_t firstInstance = 0);
        void DrawElements(uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstIndex = 0,
                          uint32_t firstInstance = 0, uint32_t vertexOffset = 0);

        void Dispatch(uint32_t x, uint32_t y = 1, uint32_t z = 1);

        template <typename T>
        void PushConstant(const std::string& name, T& constant, GraphicsStageFlags stageFlags) {
            this->PushConstant(name, static_cast<void*>(&constant), sizeof(T), stageFlags);
        }

    private:
        friend class CmdQueue;
        CmdList(std::shared_ptr<LogicalDevice> device, const CmdQueue& cmdQueue);

        const std::shared_ptr<LogicalDevice> device;
        const CmdQueue& cmdQueue;

#ifdef GRAPHICS_API_GL
        std::vector<unsigned> trackedPushConstants;
#elif defined(GRAPHICS_API_VULKAN)
        VkCommandBuffer cmdBuffer;
#endif

        void PushConstant(const std::string& name, void* constant, size_t size,
                          GraphicsStageFlags stageFlags);

        std::shared_ptr<GraphicsPipeline> boundGraphicsPipeline;
        std::shared_ptr<ComputePipeline> boundComputePipeline;
        std::vector<std::shared_ptr<RenderPass>> trackedRenderPasses;
        std::vector<std::shared_ptr<Buffer>> trackedBuffers;
        std::vector<std::shared_ptr<Image>> trackedImages;
        std::vector<std::shared_ptr<DescriptorSet>> trackedDescriptorSets;
    };
}  // namespace UniverseEngine