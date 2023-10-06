#include <math.h>

#include "CmdList.h"

namespace UniverseEngine {
    size_t GetResourceAccessBitIndex(ResourceAccessBits bits) {
        return static_cast<size_t>(log2(static_cast<float>(bits)));
    }
}  // namespace UniverseEngine