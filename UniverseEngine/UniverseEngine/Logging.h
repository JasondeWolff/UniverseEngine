#pragma once

#include <stdarg.h>

#include <mutex>

#ifdef _DEBUG
#define UE_DEBUG(fmt, ...)                                             \
    UniverseEngine::Logging::Instance().Debug(__FILE__, __LINE__, fmt, \
                                              ##__VA_ARGS__)
#else
#define UE_DEBUG(fmt, ...) ;
#endif

#define UE_INFO(fmt, ...)                                             \
    UniverseEngine::Logging::Instance().Info(__FILE__, __LINE__, fmt, \
                                             ##__VA_ARGS__)
#define UE_WARNING(fmt, ...)                                             \
    UniverseEngine::Logging::Instance().Warning(__FILE__, __LINE__, fmt, \
                                                ##__VA_ARGS__)
#define UE_ERROR(fmt, ...)                                             \
    UniverseEngine::Logging::Instance().Error(__FILE__, __LINE__, fmt, \
                                              ##__VA_ARGS__)
#define UE_FATAL(fmt, ...)                                             \
    UniverseEngine::Logging::Instance().Fatal(__FILE__, __LINE__, fmt, \
                                              ##__VA_ARGS__)

#define UE_ASSERT(condition)           \
    if (!(condition)) {                \
        UE_FATAL("Assert triggered."); \
    }
#define UE_ASSERT_MSG(condition, fmt, ...) \
    if (!(condition)) {                    \
        UE_FATAL(fmt, ##__VA_ARGS__);      \
    }

namespace UniverseEngine {
    class Logging {
    public:
        ~Logging();
        static Logging& Instance();

        void Debug(const char* file, int line, const char* fmt, ...);
        void Info(const char* file, int line, const char* fmt, ...);
        void Warning(const char* file, int line, const char* fmt, ...);
        void Error(const char* file, int line, const char* fmt, ...);
        void Fatal(const char* file, int line, const char* fmt, ...);

    private:
        Logging();

        FILE* stdOut;

        std::mutex printMutex{};

        void Print(const char* level, const char* file, int line,
                   const char* fmt, va_list args, bool showFile,
                   bool crash = false);
    };
}  // namespace UniverseEngine