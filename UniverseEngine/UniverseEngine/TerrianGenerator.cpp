#include "TerrianGenerator.h"
#include "Engine.h"
#include "Resources.h"
#include "Scene.h"

namespace UniverseEngine {
    TerrianGenerator::TerrianGenerator() {

    }

    void TerrianGenerator::CreatePlane(int width, int height, int widthSegments,
                                       int heightSegments) {
        const int width_half = width / 2;
        const int height_half = height / 2;

        const int gridX = widthSegments;
        const int gridY = heightSegments;

        const int gridX1 = gridX + 1;
        const int gridY1 = gridY + 1;

        const int segment_width = width / gridX;
        const int segment_height = height / gridY;

        Mesh chunk;
        chunk.name = "Plane" + this->generatedWorld.size();
        chunk.vertices.clear();
        chunk.vertices.reserve(gridX * gridY);

        for (int iy = 0; iy < gridY1; iy++) {
            const int y = iy * segment_height - height_half;

            for (int ix = 0; ix < gridX1; ix++) {
                const int x = ix * segment_width - width_half;

                Vertex v;
                v.position = glm::vec3(x, 0, -y);
                v.normal = Up();
                v.texCoord = glm::vec2(ix / gridX, 1 - (iy / gridY));
                v.color = glm::vec4(1, 1, 1, 1);

                chunk.vertices.emplace_back(v);
            }
        }

        chunk.indices.clear();
        chunk.indices.reserve(gridX * gridY);

        for (int iy = 0; iy < gridY; iy++) {
            for (int ix = 0; ix < gridX; ix++) {
                const uint32_t a = ix + gridX1 * iy;
                const uint32_t b = ix + gridX1 * (iy + 1);
                const uint32_t c = (ix + 1) + gridX1 * (iy + 1);
                const uint32_t d = (ix + 1) + gridX1 * iy;

                std::vector<uint32_t> tempIndices = {a, b, d, b, c, d};

                std::copy(tempIndices.begin(), tempIndices.end(), chunk.indices.end());
            }
        }

        auto hScene = Engine::GetResources().CreateScene(std::move(chunk));
        this->generatedWorld.push_back(hScene);
    }

    void TerrianGenerator::Update() {
        
    }
}  // namespace UniverseEngine