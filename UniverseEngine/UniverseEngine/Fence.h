#pragma once

#include <memory>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class LogicalDevice;

    class Fence {
    public:
        Fence(std::shared_ptr<LogicalDevice> device, bool signaled = false);
        ~Fence();
        Fence(const Fence& other) = delete;
        Fence& operator=(const Fence& other) = delete;

        bool IsComplete() const;
        void Wait() const;
        void Reset();

    private:
        const std::shared_ptr<LogicalDevice> device;

#ifdef GRAPHICS_API_VULKAN
    public:
        VkFence GetFence() const;

    private:
        VkFence fence;
#endif
    };
}  // namespace UniverseEngine