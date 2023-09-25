#pragma once

#include <bitset>

namespace UniverseEngine {
    typedef std::bitset<3> GraphicsStageFlags;
    enum GraphicsStageFlagBits { VERTEX_STAGE = 0x001, FRAGMENT_STAGE = 0x002, COMPUTE_STAGE = 0x004 };
    size_t GetGraphicsStageBitIndex(GraphicsStageFlagBits bits);
}