#include "TerrianGenerator.h"
#include "Engine.h"
#include "Resources.h"
#include "Scene.h"

namespace UniverseEngine {
    TerrianGenerator::TerrianGenerator() {
        
    }

    void TerrianGenerator::Init() {
        glm::vec2 pos = glm::vec2(0, 0);

        this->generatedWorld.resize(1);
        this->generatedWorld.at(0).resize(1);

        float width = 100;
        float height = 100;
        float widthSegments = 100;
        float heightSegments = 100;

        CreatePlane(100,100,25,25);
    }

    void TerrianGenerator::CreatePlane(int width, int height, int widthSegments,
                                       int heightSegments) {

        // Calculate step sizes
        float dx = width / static_cast<float>(widthSegments);
        float dy = height / static_cast<float>(heightSegments);

        Mesh chunk;
        chunk.name = std::string("Plane" + this->generatedWorld.size());
        chunk.vertices.clear();
        chunk.vertices.reserve(widthSegments * heightSegments);

        // Generate vertices
        for (int y = 0; y <= heightSegments; ++y) {
            for (int x = 0; x <= widthSegments; ++x) {
                Vertex vertex;

                float randOffset = (float)(rand() % 5);
                vertex.position = glm::vec3(x * dx - width / 2.0f, randOffset, y * dy - height / 2.0f);
                vertex.texCoord =
                    glm::vec2(static_cast<float>(x) / static_cast<float>(widthSegments),
                              static_cast<float>(y) / static_cast<float>(heightSegments));
                chunk.vertices.push_back(vertex);
            }
        }

        // Generate indices
        for (int y = 0; y < heightSegments; ++y) {
            for (int x = 0; x < widthSegments; ++x) {
                int vertexIndex = y * (widthSegments + 1) + x;
                chunk.indices.push_back(vertexIndex);
                chunk.indices.push_back(vertexIndex + 1);
                chunk.indices.push_back(vertexIndex + widthSegments + 1);

                chunk.indices.push_back(vertexIndex + 1);
                chunk.indices.push_back(vertexIndex + widthSegments + 2);
                chunk.indices.push_back(vertexIndex + widthSegments + 1);
            }
        }
        chunk.materialIdx = 0;

        auto hScene = Engine::GetResources().CreateScene(std::move(chunk));

        Material material;
        hScene.get()->materials.emplace_back(std::move(material));

        auto meshHierarchyRoot = hScene.get()->hierarchy.begin();
        SceneNode node{};
        node.meshIdx = std::make_optional(0);
        node.transform = Transform(glm::vec3(0, -10, 0), glm::identity<glm::quat>(), glm::vec3(1, 1, 1));
        meshHierarchyRoot = hScene.get()->hierarchy.insert(meshHierarchyRoot, node);

        Engine::GetResources().AddScene(hScene);
        this->generatedWorld.at(0).at(0) = hScene;

        int debeug = 0;
    }

    void TerrianGenerator::Update() {
        
    }
}  // namespace UniverseEngine