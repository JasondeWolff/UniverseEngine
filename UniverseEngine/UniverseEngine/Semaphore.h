#pragma once

#include <memory>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class LogicalDevice;

    class Semaphore {
    public:
        Semaphore(std::shared_ptr<LogicalDevice> device);
        ~Semaphore();
        Semaphore(const Semaphore& other) = delete;
        Semaphore& operator=(const Semaphore& other) = delete;

    private:
        const std::shared_ptr<LogicalDevice> device;

#ifdef GRAPHICS_API_VULKAN
    public:
        VkSemaphore GetSemaphore() const;

    private:
        VkSemaphore semaphore;
#endif
    };
}  // namespace UniverseEngine