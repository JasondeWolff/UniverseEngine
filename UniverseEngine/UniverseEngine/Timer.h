#pragma once

#include <chrono>

namespace UniverseEngine {
    class Timer {
    public:
        Timer();

        float Elapsed() const;
        void Reset();

    private:
        std::chrono::high_resolution_clock::time_point start;
    };
}  // namespace UniverseEngine