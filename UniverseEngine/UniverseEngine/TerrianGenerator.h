#pragma once
#include <vector>
#include <memory>
#include <glm/vec2.hpp>  // Include the appropriate header for glm::ivec2
#include <set>

#include <FastNoise/FastNoise.h>

namespace UniverseEngine {
    class Resources;
    struct SceneInstance;
    struct Scene;
    struct Mesh;

    struct TerrianGeneratorConfig {
        int chunkWidth = 1;
        int chunkHeight = 1;
        int chunkWidthSegments = 1;
        int chunkHeightSegments = 1;
        int chunkRenderDistance = 1;
    };

    struct IVec2Compare {
        bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
            if (a.x < b.x)
                return true;
            if (a.x > b.x)
                return false;
            return a.y < b.y;
        }
    };

    class TerrianGenerator {
    public:
        TerrianGenerator() = default;

        void Init(TerrianGeneratorConfig config);

    private:
        friend class Resources;
        friend class World;
        friend class Engine;

        int chunk_width;
        int chunk_height;
        int chunk_widthSegments;
        int chunk_heightSegments;
        int chunk_renderDistance;

        // Declare a set with the custom comparison function
        std::set<glm::ivec2, IVec2Compare> loadedChunks;
        std::vector<std::shared_ptr<SceneInstance>> terrian;
        glm::ivec2 previousGridPos;

        void Update();
        std::shared_ptr<Scene> CreateTerrian(int x, int y);
        void GenerateNewTerrain(int gridPosX, int gridPosY);
        Mesh* CreatePlane(int x, int y);
        float* GenerateNoise(int x, int y); 
    };
}  // namespace UniverseEngine