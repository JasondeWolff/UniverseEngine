#include "Buffer.h"

#include <math.h>

namespace UniverseEngine {
    size_t GetBufferUsageBitIndex(BufferUsageBit bit) {
        return static_cast<size_t>(log2(static_cast<float>(bit)));
    }
}