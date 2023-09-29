#include "../Sampler.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../PhysicalDevice.h"
#include "VkDebugNames.h"

namespace UniverseEngine {
    Sampler::Sampler(const std::string& name, std::shared_ptr<LogicalDevice> device,
                     const PhysicalDevice& physicalDevice)
        : device(device) {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice.GetPhysicalDevice(), &properties);

        VkSamplerCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        createInfo.magFilter = VK_FILTER_LINEAR;
        createInfo.minFilter = VK_FILTER_LINEAR;
        createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        createInfo.anisotropyEnable = VK_TRUE;
        createInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        createInfo.unnormalizedCoordinates = VK_FALSE;
        createInfo.compareEnable = VK_FALSE;
        createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        createInfo.mipLodBias = 0.0f;
        createInfo.minLod = 0.0f;
        createInfo.maxLod = 16.0f;

        UE_ASSERT_MSG(!vkCreateSampler(device->GetDevice(), &createInfo, nullptr, &this->sampler),
                      "Failed to create sampler.");

        VkDebugNames::Set(*device, VK_OBJECT_TYPE_SAMPLER,
                          reinterpret_cast<uint64_t>(this->sampler), name);
    }

    Sampler::~Sampler() {
        vkDestroySampler(this->device->GetDevice(), this->sampler, nullptr);
    }

    VkSampler Sampler::GetSampler() const {
        return this->sampler;
    }
}  // namespace UniverseEngine
#endif