#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../GraphicsInstance.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <vector>

#include "../Logging.h"

namespace UniverseEngine {
    static const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

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

    GraphicsInstance::GraphicsInstance(bool debug) {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Universe Engine App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Universe Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        if (debug) {
            CheckValidationLayerSupport();
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        UE_ASSERT_MSG(!vkCreateInstance(&createInfo, nullptr, &this->instance),
                      "Failed to create instance.");
    }

    GraphicsInstance::~GraphicsInstance() {
        vkDestroyInstance(this->instance, nullptr);
    }

}  // namespace UniverseEngine
#endif