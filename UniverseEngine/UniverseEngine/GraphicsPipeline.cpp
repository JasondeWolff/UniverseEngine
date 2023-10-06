#include <math.h>

#include "GraphicsPipeline.h"

namespace UniverseEngine {
    size_t GetPipelineStageBitIndex(PipelineStageBits bits) {
        return static_cast<size_t>(log2(static_cast<float>(bits)));
    }
}  // namespace UniverseEngine