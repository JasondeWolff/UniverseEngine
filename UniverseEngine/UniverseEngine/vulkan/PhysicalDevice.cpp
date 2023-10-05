#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <numeric>
#include <vector>

#include "../Logging.h"
#include "../PhysicalDevice.h"
#include "VkExtensions.h"

namespace UniverseEngine {
    bool IsDeviceSuitable(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);

        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
               extensionsSupported;
    }

    VkPhysicalDevice PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        UE_ASSERT_MSG(deviceCount > 0, "No vulkan compatible device found.");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (auto& device : devices) {
            if (IsDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
        }

        UE_ASSERT_MSG(physicalDevice != VK_NULL_HANDLE, "No vulkan compatible device found.");

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        UE_INFO("Using Device: %s", deviceProperties.deviceName);

        return physicalDevice;
    }

    PhysicalDevice::PhysicalDevice(const GraphicsInstance& instance) {
        this->physicalDevice = PickPhysicalDevice(instance.GetInstance(), instance.GetSurface());

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(this->physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(this->physicalDevice, &queueFamilyCount,
                                                 queueFamilies.data());

        this->graphicsFamily = UINT_MAX;
        this->computeFamily = UINT_MAX;
        this->presentFamily = UINT_MAX;
        for (size_t i = 0; i < queueFamilyCount; i++) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                this->graphicsFamily = static_cast<uint32_t>(i);
            }

            if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
                this->computeFamily = static_cast<uint32_t>(i);
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(this->physicalDevice, static_cast<uint32_t>(i),
                                                 instance.GetSurface(), &presentSupport);
            if (presentSupport) {
                this->presentFamily = static_cast<uint32_t>(i);
            }

            if (this->graphicsFamily != UINT_MAX && this->computeFamily != UINT_MAX &&
                this->presentFamily != UINT_MAX) {
                return;
            }
        }

        UE_FATAL("Missing queue families!");
    }

    VkPhysicalDevice PhysicalDevice::GetPhysicalDevice() const {
        return this->physicalDevice;
    }

    uint32_t PhysicalDevice::GraphicsFamily() const {
        return this->graphicsFamily;
    }

    uint32_t PhysicalDevice::ComputeFamily() const {
        return this->computeFamily;
    }

    uint32_t PhysicalDevice::PresentFamily() const {
        return this->presentFamily;
    }

    uint32_t PhysicalDevice::FindMemoryType(uint32_t typeFilter,
                                            VkMemoryPropertyFlags properties) const {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(this->physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        UE_FATAL("Failed to find suitable memory type.");
        return 0;
    }
}  // namespace UniverseEngine
#endif