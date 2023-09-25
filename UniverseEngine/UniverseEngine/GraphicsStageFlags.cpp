#include "GraphicsStageFlags.h"

#include <math.h>

namespace UniverseEngine {
    size_t GetGraphicsStageBitIndex(GraphicsStageFlagBits bits) {
        return static_cast<size_t>(log2(static_cast<float>(bits)));
    }
}