#include "Plane.h"
#include "MathUtil.h"

namespace UniverseEngine{
    Plane::Plane(int width, int height, int widthSegments, int heightSegments)
        : m_Width(width), m_Height(height), m_WidthSegments(widthSegments), m_HeightSegments(heightSegments)
    {
       /* const int width_half = width / 2;
        const int height_half = height / 2;

        const int gridX = widthSegments;
        const int gridY = heightSegments;

        const int gridX1 = gridX + 1;
        const int gridY1 = gridY + 1;

        const int segment_width = width / gridX;
        const int segment_height = height / gridY;

        name = "Plane";

        for (int iy = 0; iy < gridY1; iy++) {
            const int y = iy * segment_height - height_half;

            for (int ix = 0; ix < gridX1; ix++) {
                const int x = ix * segment_width - width_half;

                Vertex v;
                v.position = {x, 0, -y};
                v.normal = Up();
                v.texCoord = {ix / gridX, 1 - (iy / gridY)};
                v.color = {1, 1, 1};
                vertices.emplace_back(v);
            }
        }

        for (int iy = 0; iy < gridY; iy++) {
            for (int ix = 0; ix < gridX; ix++) {
                const int a = ix + gridX1 * iy;
                const int b = ix + gridX1 * (iy + 1);
                const int c = (ix + 1) + gridX1 * (iy + 1);
                const int d = (ix + 1) + gridX1 * iy;

                indices.emplace_back(a, b, d);
                indices.emplace_back(b, c, d);
            }
        }*/
    }
}
