#pragma once
#include <vector>
#include <memory>

namespace UniverseEngine {
    class Resources;
    struct Scene;

    class TerrianGenerator {
    public:
        TerrianGenerator();
        void CreatePlane(int width = 1, int height = 1, int widthSegments = 1,
                         int heightSegments = 1);

    private:
        friend class Resources;

        std::vector<std::shared_ptr<Scene>> generatedWorld;

        void Update();
    };
}  // namespace UniverseEngine