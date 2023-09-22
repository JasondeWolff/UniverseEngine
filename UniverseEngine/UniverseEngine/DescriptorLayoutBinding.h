#pragma once

#include <bitset>
#include <memory>
#include <string>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    enum class DescriptorType {
        SAMPLER,
        COMBINED_IMAGE_SAMPLER,
        SAMPLED_IMAGE,
        STORAGE_IMAGE,
        UNIFORM_BUFFER,
        STORAGE_BUFFER
    };

    typedef std::bitset<3> DescriptorStageFlags;
    enum DescriptorStageFlagBits { VERTEX = 0x001, FRAGMENT = 0x002, COMPUTE = 0x004 };
    size_t GetDescriptorStageBitIndex(DescriptorStageFlagBits bits);

    struct DescriptorLayoutBinding {
        DescriptorLayoutBinding(const std::string& name, uint32_t binding, DescriptorType type,
                                DescriptorStageFlags stageFlags)
            : name(name), binding(binding), type(type), stageFlags(stageFlags) {
        }

        const std::string name;
        const uint32_t binding;
        const DescriptorType type;
        const DescriptorStageFlags stageFlags;
    };
}  // namespace UniverseEngine