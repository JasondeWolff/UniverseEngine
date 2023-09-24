#include "Image.h"

#include <math.h>

namespace UniverseEngine {
    size_t GetImageUsageBitIndex(ImageUsageBits bits) {
        return static_cast<size_t>(log2(static_cast<float>(bits)));
    }

    uint32_t Image::Width() const {
        return this->width;
    }

    uint32_t Image::Height() const {
        return this->height;
    }

    GraphicsFormat Image::Format() const {
        return this->format;
    }
}