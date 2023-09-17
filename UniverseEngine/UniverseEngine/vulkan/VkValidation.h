#pragma once

#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <vector>

namespace UniverseEngine {
    static const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    void CheckValidationLayerSupport();
}
#endif