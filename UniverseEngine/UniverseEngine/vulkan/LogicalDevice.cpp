#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Logging.h"
#include "../LogicalDevice.h"
#include "VkExtensions.h"
#include "VkValidation.h"

namespace UniverseEngine {
    LogicalDevice::LogicalDevice(std::shared_ptr<GraphicsInstance> instance,
                                 const PhysicalDevice& physicalDevice, bool debug)
        : instance(instance) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = physicalDevice.GraphicsFamily();
        queueCreateInfo.queueCount = 1;
        float queuePriority = 1.0;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        // TODO: Move as a getter from physicalDevice
        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (debug) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        UE_ASSERT_MSG(!vkCreateDevice(physicalDevice.GetPhysicalDevice(), &createInfo, nullptr,
                                      &this->device),
                      "Failed to create logical device.");
    }

    LogicalDevice::~LogicalDevice() {
        this->WaitIdle();
        vkDestroyDevice(this->device, nullptr);
    }

    void LogicalDevice::WaitIdle() const {
        vkDeviceWaitIdle(this->device);
    }

    VkDevice LogicalDevice::GetDevice() const {
        return this->device;
    }
}  // namespace UniverseEngine
#endif