#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <vector>

#include "../GraphicsInstance.h"
#include "../Logging.h"
#include "../PhysicalDevice.h"
#include "VkValidation.h"

namespace UniverseEngine {
    VkInstance CreateInstance(bool debug) {
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

        VkInstance instance;
        UE_ASSERT_MSG(!vkCreateInstance(&createInfo, nullptr, &instance),
                      "Failed to create instance.");
        return instance;
    }

    VkSurfaceKHR CreateSurface(GLFWwindow* glfwWindow, VkInstance instance) {
        VkWin32SurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd = glfwGetWin32Window(glfwWindow);
        createInfo.hinstance = GetModuleHandle(nullptr);

        VkSurfaceKHR surface;
        UE_ASSERT_MSG(!vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface),
                      "Failed to create surface.");
        return surface;
    }

    GraphicsInstance::GraphicsInstance(const Window& window, bool debug) {
        this->instance = CreateInstance(debug);
        this->surface = CreateSurface(window.GlfwWindow(), this->instance);
    }

    GraphicsInstance::~GraphicsInstance() {
        vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
        vkDestroyInstance(this->instance, nullptr);
    }

    VkInstance GraphicsInstance::GetInstance() const {
        return this->instance;
    }

    VkSurfaceKHR GraphicsInstance::GetSurface() const {
        return this->surface;
    }
}  // namespace UniverseEngine
#endif