#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <string>

namespace UniverseEngine {
    inline std::string Format(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        char t[1024 * 12];
        vsnprintf(t, sizeof(t), fmt, args);
        va_end(args);
        return std::string(t);
    }
}