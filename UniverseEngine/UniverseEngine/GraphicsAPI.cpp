#include "GraphicsAPI.h"

#ifdef GRAPHICS_API_GL
#include <GL/glew.h>

namespace UniverseEngine {
    void GraphicsAPI::Init() {
        glewInit();
    }
}
#endif