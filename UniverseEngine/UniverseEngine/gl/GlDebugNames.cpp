#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Logging.h"
#include "GlDebugNames.h"

namespace UniverseEngine {
    void GlDebugNames::Set(GLenum identifier, GLuint object, const std::string& name) {
        GLint maxLabelLength;
        glGetIntegerv(GL_MAX_LABEL_LENGTH, &maxLabelLength);
        UE_ASSERT_MSG(name.size() < maxLabelLength, "Debug name too long.");

        glObjectLabel(identifier, object, static_cast<GLsizei>(name.size()), name.c_str());
    }

    std::string GlDebugNames::Get(GLenum identifier, GLuint object) {
        char name[256];
        GLsizei length;
        glGetObjectLabel(identifier, object, sizeof(name), &length, name);
        return std::string(name);
    }
}  // namespace UniverseEngine
#endif