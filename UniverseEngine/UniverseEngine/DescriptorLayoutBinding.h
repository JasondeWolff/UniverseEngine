#pragma once

#include <bitset>
#include <memory>
#include <string>

#include "GraphicsAPI.h"
#include "GraphicsStageFlags.h"

namespace UniverseEngine {
    enum class DescriptorType {
        SAMPLER,
        COMBINED_IMAGE_SAMPLER,
        SAMPLED_IMAGE,
        STORAGE_IMAGE,
        UNIFORM_BUFFER,
        STORAGE_BUFFER
    };

    struct DescriptorLayoutBinding {
        DescriptorLayoutBinding(const std::string& name, uint32_t binding, DescriptorType type,
                                GraphicsStageFlags stageFlags)
            : name(name), binding(binding), type(type), stageFlags(stageFlags) {
        }

        const std::string name;
        const uint32_t binding;
        const DescriptorType type;
        const GraphicsStageFlags stageFlags;
    };
}  // namespace UniverseEngine