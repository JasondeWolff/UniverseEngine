#include "Logging.h"

#include <assert.h>
#include <fstream>
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace UniverseEngine {
    Logging::Logging() {
        this->stdOut = freopen(".stdlog", "w", stderr);
    }

    Logging::~Logging() {
        fflush(this->stdOut);
    }

    Logging& Logging::Instance() {
        static Logging logger;
        return logger;
    }

    void Logging::Debug(const char* file, int line, const char* fmt, ...) {
        std::lock_guard<std::mutex> lock(printMutex);
        va_list args;
        va_start(args, fmt);
        Print("Debug", file, line, fmt, args, true);
        va_end(args);
    }

    void Logging::Info(const char* file, int line, const char* fmt, ...) {
        std::lock_guard<std::mutex> lock(printMutex);
        va_list args;
        va_start(args, fmt);
        Print("Info", file, line, fmt, args, false);
        va_end(args);
    }

    void Logging::Warning(const char* file, int line, const char* fmt, ...) {
        std::lock_guard<std::mutex> lock(printMutex);
        va_list args;
        va_start(args, fmt);
        Print("Warning", file, line, fmt, args, false);
        va_end(args);
    }

    void Logging::Error(const char* file, int line, const char* fmt, ...) {
        std::lock_guard<std::mutex> lock(printMutex);
        va_list args;
        va_start(args, fmt);
        Print("Error", file, line, fmt, args, true);
        va_end(args);
    }

    void Logging::Fatal(const char* file, int line, const char* fmt, ...) {
        std::lock_guard<std::mutex> lock(printMutex);
        va_list args;
        va_start(args, fmt);
        Print("Fatal", file, line, fmt, args, true, true);
        va_end(args);
    }

    void Logging::Print(const char* level, const char* file, int line,
                        const char* fmt, va_list args, bool showFile,
                        bool crash) {
        char t[16000];
        vsnprintf(t, sizeof(t), fmt, args);

        {
            std::string f(file);
            fprintf(stdout, "[%s]", level);
            if (showFile)
                fprintf(stdout, "[%s: %d]",
                        f.substr(f.find_last_of("/\\") + 1).c_str(), line);

            fprintf(stdout, " ");
            fprintf(stdout, t);
            fprintf(stdout, "\n");
        }

        /*{
            FILE* fp = fopen(".log", "w+");
            std::string f(file);
            fprintf(fp, "[%s]", level);
            if (showFile)
                fprintf(fp, "[%s: %d]", f.substr(f.find_last_of("/\\") + 1).c_str(), line);

            fprintf(fp, " ");
            fprintf(fp, t);
            fprintf(fp, "\n");
            fflush(fp);
        }*/

        if (crash) {
            __debugbreak();
            exit(EXIT_FAILURE);
        }
    }
}  // namespace UniverseEngine