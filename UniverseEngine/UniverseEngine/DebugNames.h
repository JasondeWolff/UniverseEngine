#pragma once

#include <GL/glew.h>

#include <string>

namespace UniverseEngine {
    class DebugNames {
    public:
        static void Set(GLenum identifier, GLuint object, const std::string& name);
        static std::string Get(GLenum identifier, GLuint object);
    };
}  // namespace UniverseEngine