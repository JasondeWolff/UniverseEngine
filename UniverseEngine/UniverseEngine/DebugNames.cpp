#include "DebugNames.h"

namespace UniverseEngine {
    void DebugNames::Set(GLenum identifier, GLuint object, const std::string& name) {
        glObjectLabel(identifier, object, static_cast<GLsizei>(name.size()), name.c_str());
    }

    std::string DebugNames::Get(GLenum identifier, GLuint object) {
        char name[1024];
        GLsizei length;
        glGetObjectLabel(identifier, object, sizeof(name), &length, name);
        return std::string(name);
    }
}