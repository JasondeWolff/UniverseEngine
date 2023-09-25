#pragma once

#include <string>

#include "GraphicsAPI.h"
#include "GraphicsStageFlags.h"

namespace UniverseEngine {
    struct PushConstantRange {
        PushConstantRange(const std::string& name, size_t size, GraphicsStageFlags stageFlags)
            : name(name), size(size), stageFlags(stageFlags) {
        }

        const std::string name;
        const size_t size;
        const GraphicsStageFlags stageFlags;
    };
}  // namespace UniverseEngine