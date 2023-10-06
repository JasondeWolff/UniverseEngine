#pragma once

#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <vector>
#include <unordered_map>

namespace UniverseEngine {
    class Image;

    struct ImageState {
        VkImageLayout layout;
        VkAccessFlags accessFlags;
        VkPipelineStageFlags stageFlags;
    };

    class VkResourceStateTracker {
    public:
        static void TransitionImage(VkCommandBuffer cmdBuffer, const Image& image, ImageState newState);

        static void AddGlobalImageState(VkImage image, ImageState state);
        static void RemoveGlobalImageState(VkImage image);

    private:
        std::unordered_map<VkImage, ImageState> imageStates;
        static std::unordered_map<VkImage, ImageState> globalImageStates;
    };
}  // namespace UniverseEngine
#endif