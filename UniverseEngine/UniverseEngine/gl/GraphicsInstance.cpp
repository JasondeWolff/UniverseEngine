#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../GraphicsInstance.h"

#include <GL/glew.h>

#include "../Logging.h"

namespace UniverseEngine {
    void GlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                         const GLchar* message, const void* userParam) {
        UE_ASSERT_MSG(severity != GL_DEBUG_SEVERITY_HIGH, "GL: %s", message);
    }

    GraphicsInstance::GraphicsInstance(bool debug) {
        glewInit();

        if (debug) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GlDebugCallback, nullptr);
        }
    }

    GraphicsInstance::~GraphicsInstance() {
    }
}  // namespace UniverseEngine
#endif