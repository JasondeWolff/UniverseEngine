#include "Buffer.h"

#include <math.h>

namespace UniverseEngine {
    size_t GetBufferUsageBitIndex(BufferUsageBits bits) {
        return static_cast<size_t>(log2(static_cast<float>(bits)));
    }
}