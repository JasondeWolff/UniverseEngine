#pragma once

#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include <GL/glew.h>

#include <string>

namespace UniverseEngine {
    class GlDebugNames {
    public:
        static void Set(GLenum identifier, GLuint object, const std::string& name);
        static std::string Get(GLenum identifier, GLuint object);
    };
}  // namespace UniverseEngine
#endif