#include <math.h>

#include "DescriptorLayoutBinding.h"

namespace UniverseEngine {
    size_t GetDescriptorStageBitIndex(DescriptorStageFlagBits bits) {
        return static_cast<size_t>(log2(static_cast<float>(bits)));
    }
}  // namespace UniverseEngine