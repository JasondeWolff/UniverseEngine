#pragma once
#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include <map>

namespace UniverseEngine {
    class Resources;
    struct Scene;

    class TerrianGenerator {
    public:
        TerrianGenerator(){};

        void Init(int width = 1, int height = 1, int widthSegments = 1, int heightSegments = 1);
        std::shared_ptr<Scene> CreatePlane(glm::vec2 gridPos);

        std::vector<std::vector<std::shared_ptr<Scene>>> newlyGeneratedWorld;
    private:
        friend class Resources;
        friend class World;
        friend class Engine;

        int chunk_width;
        int chunk_height;
        int chunk_widthSegments;
        int chunk_heightSegments;

        std::vector<std::vector<std::shared_ptr<Scene>>> generatedWorld;

        void ResizeVectors(int newWidth, int newHeight);
        void Update();
    };
}  // namespace UniverseEngine