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
        TerrianGenerator();
        void Init(int width = 1, int height = 1, int widthSegments = 1, int heightSegments = 1);
        void CreatePlane(glm::vec2 gridPos);

    private:
        friend class Resources;
        friend class World;
        friend class Engine;

        int chunk_width = 100;
        int chunk_height = 100;
        int chunk_widthSegments = 25;
        int chunk_heightSegments = 25;

        //std::map<unsigned int>
        //std::map <unsigned int, std::map<unsigned int, std::shared_ptr<Scene>>> generatedWorld;
        //std::map<std::pair<unsigned int, unsigned int>, std::shared_ptr<Scene>> generatedWorld;
        std::map<uint32_t, std::map<uint32_t, std::shared_ptr<Scene>>> generatedWorld;
        void Update();
    };
}  // namespace UniverseEngine