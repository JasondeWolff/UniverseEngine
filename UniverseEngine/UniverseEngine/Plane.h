#pragma once
#include "Mesh.h"

namespace UniverseEngine {
    class Plane : Mesh {
    public:
        Plane(int width = 1, int height = 1, int widthSegments = 1, int heightSegments = 1);
        ~Plane();

    private:
        int m_Width;
        int m_Height;
        int m_WidthSegments;
        int m_HeightSegments;
    };
}  // namespace UniverseEngine
