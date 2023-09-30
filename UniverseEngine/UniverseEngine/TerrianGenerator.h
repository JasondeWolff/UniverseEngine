#pragma once
#include <vector>
#include <memory>

namespace UniverseEngine {
    class Resources;
    struct Scene;

    class TerrianGenerator {
    public:
        TerrianGenerator();
        void Init();

        void CreatePlane(int width = 1, int height = 1, int widthSegments = 1,
                         int heightSegments = 1);

    private:
        friend class Resources;
        friend class World;
        friend class Engine;

        std::vector<std::vector<std::shared_ptr<Scene>>> generatedWorld;

        void Update();
    };
}  // namespace UniverseEngine