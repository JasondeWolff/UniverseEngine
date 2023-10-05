#include "TerrianGenerator.h"

#include "Engine.h"
#include "Resources.h"
#include "Scene.h"
#include "Camera.h"

#include <iostream>
#include <string>
#include <vector>

namespace UniverseEngine {
    TerrianGenerator::TerrianGenerator() {
    }

    void TerrianGenerator::Init(int width, int height, int widthSegments, int heightSegments)
    {
        this->chunk_width = width;
        this->chunk_height = height;
        this->chunk_widthSegments = widthSegments;
        this->chunk_heightSegments = heightSegments;
        
        CreatePlane(glm::vec2(0,0));
    }

    void TerrianGenerator::CreatePlane(glm::vec2 gridPos) {

        // Calculate step sizes
        float dx = chunk_width / static_cast<float>(chunk_widthSegments);
        float dy = chunk_height / static_cast<float>(chunk_heightSegments);

        Mesh chunk;
        chunk.name = std::string("Plane");
        chunk.vertices.clear();
        chunk.vertices.reserve(chunk_widthSegments * chunk_heightSegments);

        // Generate vertices
        for (int y = 0; y <= chunk_widthSegments; ++y) {
            for (int x = 0; x <= chunk_heightSegments; ++x) {
                Vertex vertex;

                float randOffset = (float)(rand() % 5);
                vertex.position = glm::vec3(x * dx - chunk_width / 2.0f, randOffset,
                                            y * dy - chunk_height / 2.0f);
                vertex.texCoord =
                    glm::vec2(static_cast<float>(x) / static_cast<float>(chunk_widthSegments),
                              static_cast<float>(y) / static_cast<float>(chunk_heightSegments));
                chunk.vertices.push_back(vertex);
            }
        }

        // Generate indices
        for (int y = 0; y < chunk_heightSegments; ++y) {
            for (int x = 0; x < chunk_widthSegments; ++x) {
                int vertexIndex = y * (chunk_widthSegments + 1) + x;
                chunk.indices.push_back(vertexIndex);
                chunk.indices.push_back(vertexIndex + 1);
                chunk.indices.push_back(vertexIndex + chunk_widthSegments + 1);

                chunk.indices.push_back(vertexIndex + 1);
                chunk.indices.push_back(vertexIndex + chunk_widthSegments + 2);
                chunk.indices.push_back(vertexIndex + chunk_widthSegments + 1);
            }
        }
        chunk.materialIdx = 0;

        auto hScene = Engine::GetResources().CreateScene(std::move(chunk));

        Material material;
        hScene.get()->materials.emplace_back(std::move(material));

        auto meshHierarchyRoot = hScene.get()->hierarchy.begin();
        SceneNode node{};
        node.meshIdx = std::make_optional(0);
        node.transform = Transform(glm::vec3(gridPos.x * chunk_width, -50,
                                gridPos.y * chunk_width), 
            glm::identity<glm::quat>(), 
            glm::vec3(1, 1, 1)
        );
        meshHierarchyRoot = hScene.get()->hierarchy.insert(meshHierarchyRoot, node);

        
        //this->generatedWorld[(unsigned int)gridPos.x][(unsigned int)gridPos.y] = hScene;
        /*this->generatedWorld.insert(std::pair < std::map<unsigned int, std::shared_ptr<Scene>>(
                                                    (unsigned int)gridPos.y, hScene));*/
        this->generatedWorld[(uint32_t)gridPos.x][(uint32_t)gridPos.y] = hScene;
    }

    void TerrianGenerator::Update() {
        auto camera = Engine::GetWorld().camera;
        glm::vec3 camerePos = camera.transform.GetTranslation();
        glm::vec2 gridPos =
        { 
            camerePos.x / chunk_width,
            camerePos.y / chunk_height 
        };

        if (this->generatedWorld[(uint32_t)gridPos.x][(uint32_t)gridPos.y] == nullptr)
        {
            CreatePlane(gridPos);
        }

        int debug = 0;
    }
}  // namespace UniverseEngine