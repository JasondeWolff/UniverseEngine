#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "VkValidation.h"

#include "../Logging.h"

namespace UniverseEngine {
    void CheckValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            UE_ASSERT_MSG(layerFound, "Validation layer '%s' is not supported.", layerName);
        }
    }
}
#endif