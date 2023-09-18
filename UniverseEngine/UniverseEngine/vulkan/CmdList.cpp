#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../CmdList.h"

#include "../GraphicsPipeline.h"
#include "../Logging.h"
#include "../CmdQueue.h"

namespace UniverseEngine {
    CmdList::CmdList(std::shared_ptr<LogicalDevice> device, const CmdQueue& cmdQueue) : device(device) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = cmdQueue.GetCmdPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        UE_ASSERT_MSG(!vkAllocateCommandBuffers(device->GetDevice(), &allocInfo, &this->cmdBuffer),
                      "Failed to allocate command buffer.");
    }

    CmdList::~CmdList() {
    }

    void CmdList::Reset() {
        this->graphicsPipeline.reset();
    }

    void CmdList::Clear(const glm::vec4& clearColor) {
    }

    void CmdList::SetScissor(const Rect2D& rect2D) {
    }

    void CmdList::SetViewport(const Rect2D& rect2D) {
    }

    void CmdList::BindGraphicsPipeline(std::shared_ptr<GraphicsPipeline> graphicsPipeline) {
        this->graphicsPipeline = graphicsPipeline;
    }

    void CmdList::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,
                       uint32_t firstInstance) {
    }

    void CmdList::DrawElements(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                               uint32_t firstInstance) {
    }

    void CmdList::PushConstant(const std::string& name, void* constant, size_t size) {
    }
}  // namespace UniverseEngine
#endif