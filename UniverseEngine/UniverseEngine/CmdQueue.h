#pragma once

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class CmdQueue {
    public:
        CmdQueue();
        ~CmdQueue();

        CmdQueue(const CmdQueue& other) = delete;
        CmdQueue& operator=(const CmdQueue& other) = delete;
    };
}  // namespace UniverseEngine