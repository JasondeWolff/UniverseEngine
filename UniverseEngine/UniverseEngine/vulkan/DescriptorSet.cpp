#include "../DescriptorSet.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Buffer.h"
#include "../DescriptorPool.h"
#include "../DescriptorSetLayout.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../Image.h"
#include "../Sampler.h"
#include "VkConversion.h"

namespace UniverseEngine {
    DescriptorSet::DescriptorSet(const std::shared_ptr<LogicalDevice> device,
                                 const std::shared_ptr<DescriptorPool> pool,
                                 const std::shared_ptr<DescriptorSetLayout> layout)
        : device(device), pool(pool), layout(layout) {
        VkDescriptorSetLayout vkLayouts[] = {layout->GetLayout()};

        VkDescriptorSetAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocateInfo.descriptorPool = pool->GetPool();
        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = vkLayouts;

        UE_ASSERT_MSG(
            !vkAllocateDescriptorSets(device->GetDevice(), &allocateInfo, &this->descriptorSet),
            "Failed to allocate descriptor set.");
    }

    DescriptorSet::~DescriptorSet() {
        vkFreeDescriptorSets(this->device->GetDevice(), this->pool->GetPool(), 1,
                             &this->descriptorSet);
    }

    void DescriptorSet::SetBuffer(uint32_t binding, DescriptorType type,
                                  std::shared_ptr<Buffer> buffer) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = buffer->GetBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = buffer->Size();

        VkWriteDescriptorSet writeInfo{};
        writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfo.dstSet = this->descriptorSet;
        writeInfo.dstBinding = binding;
        writeInfo.descriptorCount = 1;
        writeInfo.descriptorType = GetVkDescriptorType(type);
        writeInfo.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(this->device->GetDevice(), 1, &writeInfo, 0, nullptr);
    }

    void DescriptorSet::SetImage(uint32_t binding, DescriptorType type, std::shared_ptr<Image> image, std::shared_ptr<Sampler> sampler) {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = image->GetImageView();
        imageInfo.sampler = sampler->GetSampler();

        VkWriteDescriptorSet writeInfo{};
        writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfo.dstSet = this->descriptorSet;
        writeInfo.dstBinding = binding;
        writeInfo.descriptorCount = 1;
        writeInfo.descriptorType = GetVkDescriptorType(type);
        writeInfo.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(this->device->GetDevice(), 1, &writeInfo, 0, nullptr);
    }

    VkDescriptorSet DescriptorSet::GetDescriptorSet() const {
        return this->descriptorSet;
    }
}  // namespace UniverseEngine
#endif