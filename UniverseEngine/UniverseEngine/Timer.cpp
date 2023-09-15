#include "Timer.h"

using namespace std::chrono;

namespace UniverseEngine {
    Timer::Timer() {
        this->Reset();
    }

    float Timer::Elapsed() const {
        high_resolution_clock::time_point t2 =
            high_resolution_clock::now();
        duration<double> time_span =
            duration_cast<duration<double>>(t2 - this->start);
        return static_cast<float>(time_span.count());
    }

    void Timer::Reset() {
        start = high_resolution_clock::now();
    }
}