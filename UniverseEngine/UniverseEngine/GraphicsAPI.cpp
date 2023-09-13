#include "GraphicsAPI.h"

#ifdef GRAPHICS_API_GL
#include <GL/glew.h>

#include "Logging.h"

namespace UniverseEngine {
    void GlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                         const GLchar* message, const void* userParam) {
        UE_FATAL("GL: %s", message);
    }

    void GraphicsAPI::Init() {
        glewInit();

#ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GlDebugCallback, nullptr);
#endif
    }

    const char* GraphicsAPI::ShaderDir() {
        return "GL/";
    }
}  // namespace UniverseEngine
#endif