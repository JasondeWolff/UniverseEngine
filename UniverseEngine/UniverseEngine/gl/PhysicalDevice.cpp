#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <vector>
#include <numeric>

#include "../PhysicalDevice.h"
#include "../Logging.h"

namespace UniverseEngine {
    bool IsDeviceSuitable(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
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
        for (size_t i = 0; i < queueFamilyCount; i++) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                this->graphicsFamily = static_cast<uint32_t>(i);
            }

            if (this->graphicsFamily != UINT_MAX) {
                return;
            }
        }
    }

    VkPhysicalDevice PhysicalDevice::GetPhysicalDevice() const {
        return this->physicalDevice;
    }
}  // namespace UniverseEngine
#endif