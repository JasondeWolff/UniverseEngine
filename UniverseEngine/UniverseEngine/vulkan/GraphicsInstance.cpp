#include "../GraphicsInstance.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "../Logging.h"

namespace UniverseEngine {
    GraphicsInstance::GraphicsInstance() {
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
        createInfo.enabledLayerCount = 0;

        UE_ASSERT_MSG(!vkCreateInstance(&createInfo, nullptr, &this->instance), "Failed to create instance.");
    }

    GraphicsInstance::~GraphicsInstance() {
        vkDestroyInstance(this->instance, nullptr);
    }
}