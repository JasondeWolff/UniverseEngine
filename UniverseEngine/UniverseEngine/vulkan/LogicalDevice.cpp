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

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.fillModeNonSolid = VK_TRUE;
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkPhysicalDeviceBufferDeviceAddressFeaturesKHR bufferDeviceAddressFeatures{};
        bufferDeviceAddressFeatures.sType =
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_KHR;
        bufferDeviceAddressFeatures.bufferDeviceAddress = true;
        bufferDeviceAddressFeatures.bufferDeviceAddressCaptureReplay = true;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pNext = &bufferDeviceAddressFeatures;
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

        VmaAllocatorCreateInfo vmaCreateInfo{};
        vmaCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
        vmaCreateInfo.instance = instance->GetInstance();
        vmaCreateInfo.physicalDevice = physicalDevice.GetPhysicalDevice();
        vmaCreateInfo.device = this->device;
        vmaCreateInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

        UE_ASSERT_MSG(!vmaCreateAllocator(&vmaCreateInfo, &this->allocator),
                      "Failed to create allocator.");
    }

    LogicalDevice::~LogicalDevice() {
        this->WaitIdle();
        vmaDestroyAllocator(this->allocator);
        vkDestroyDevice(this->device, nullptr);
    }

    void LogicalDevice::WaitIdle() const {
        vkDeviceWaitIdle(this->device);
    }

    VkDevice LogicalDevice::GetDevice() const {
        return this->device;
    }

    VmaAllocator LogicalDevice::GetAllocator() const {
        return this->allocator;
    }
}  // namespace UniverseEngine
#endif