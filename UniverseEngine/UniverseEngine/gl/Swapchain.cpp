#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Format.h"
#include "../Swapchain.h"

namespace UniverseEngine {
    Swapchain::Swapchain(const Window& window, const GraphicsInstance& instance,
                         std::shared_ptr<LogicalDevice> device,
                         const PhysicalDevice& physicalDevice)
        : device(device), currentFrame(0), currentImage(0) {
        this->width = window.Width();
        this->height = window.Height();

        this->image = std::make_shared<Image>(
            "SwapchainFramebuffer", this->device, physicalDevice, this->width, this->height, 1,
            ImageUsageBits::STORAGE_IMAGE, GraphicsFormat::R8G8B8A8_SRGB);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            imageAvailableSemaphores.emplace_back(
                std::move(Semaphore(UniverseEngine::Format("Image Available %i", i), device)));
            renderFinishedSemaphores.emplace_back(
                std::move(Semaphore(UniverseEngine::Format("Render Finished %i", i), device)));
            inflightFences.emplace_back(std::move(std::make_shared<Fence>(device, true)));
        }

        glEnable(GL_FRAMEBUFFER_SRGB);
    }

    Swapchain::~Swapchain() {
    }

    const Framebuffer& Swapchain::GetCurrentFramebuffer() const {
        return *this->framebuffer;
    }

    const Image& Swapchain::GetCurrentImage() const {
        return *this->framebuffer->Images()[0];
    }

    uint32_t Swapchain::GetCurrentFrameIdx() const {
        return this->currentFrame;
    }

    Semaphore& Swapchain::GetImageAvailableSemaphore() {
        return this->imageAvailableSemaphores[static_cast<size_t>(this->currentFrame)];
    }

    Semaphore& Swapchain::GetRenderFinishedSemaphore() {
        return this->renderFinishedSemaphores[static_cast<size_t>(this->currentFrame)];
    }

    void Swapchain::RebuildFramebuffers(std::shared_ptr<RenderPass> renderPass,
                                        std::shared_ptr<Image> depthImage) {
        this->framebuffer.reset();
        this->framebuffer = std::move(std::make_unique<Framebuffer>(
            this->device, std::vector<std::shared_ptr<Image>>{this->image}, renderPass));
    }

    std::shared_ptr<Fence> Swapchain::NextImage() {
        return std::make_shared<Fence>(this->device);
    }

    void Swapchain::Present(const CmdQueue& queue, const Fence& fence,
                            const std::vector<Semaphore*>& semaphores) {
    }

    GraphicsFormat Swapchain::Format() const {
        return GraphicsFormat::UNDEFINED;
    }

    Rect2D Swapchain::Extent() const {
        return Rect2D(this->width, this->height);
    }
}  // namespace UniverseEngine
#endif