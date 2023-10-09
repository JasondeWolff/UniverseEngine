#pragma once
#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include <map>

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

    class TerrianGenerator {
    public:
        TerrianGenerator() = default;

        void Init(TerrianGeneratorConfig config);

        std::vector<std::shared_ptr<SceneInstance>> newlyGeneratedWorld;
    private:
        friend class Resources;
        friend class World;
        friend class Engine;

        int chunk_width;
        int chunk_height;
        int chunk_widthSegments;
        int chunk_heightSegments;
        int chunk_renderDistance;

        std::vector<std::shared_ptr<SceneInstance>> generatedWorld;

        void Update();
        std::shared_ptr<Scene> CreateScene(int x, int y);
        Mesh* CreatePlane(int x, int y);
        float* GenerateNoise(int x, int y); 
    };
}  // namespace UniverseEngine