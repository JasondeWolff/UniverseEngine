#include "Image.h"

#include <math.h>

namespace UniverseEngine {
    size_t GetImageUsageBitIndex(ImageUsageBits bits) {
        return static_cast<size_t>(log2(static_cast<float>(bits)));
    }
}