#include "TerrianGenerator.h"

#include "Engine.h"
#include "Resources.h"
#include "Scene.h"
#include "Camera.h"

#include <iostream>
#include <string>
#include <vector>

namespace UniverseEngine {

    void TerrianGenerator::Init(int width, int height, int widthSegments, int heightSegments)
    {
        this->chunk_width = width;
        this->chunk_height = height;
        this->chunk_widthSegments = widthSegments;
        this->chunk_heightSegments = heightSegments;
        
        //CreatePlane(glm::vec2(0,0));
    }

    std::shared_ptr<Scene> TerrianGenerator::CreatePlane(glm::vec2 gridPos) {

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
        
        return hScene;
    }

    void TerrianGenerator::ResizeVectors(int newWidth, int newHeight) {
        // Resize the generatedWorld vector
        this->generatedWorld.resize(newWidth, std::vector<std::shared_ptr<Scene>>(newHeight, nullptr));

        // Resize the newlyGeneratedWorld vector (if needed)
        this->newlyGeneratedWorld.resize(newWidth, std::vector<std::shared_ptr<Scene>>(newHeight, nullptr));
    }

    void TerrianGenerator::Update() {
        auto camera = Engine::GetWorld().camera;
        glm::vec3 cameraPos = camera.transform.GetTranslation();

        const glm::ivec2 gridPos = {
            static_cast<int>(cameraPos.x) / chunk_width,
            static_cast<int>(cameraPos.y) / chunk_height
        };

        // Check if the vectors need resizing
        if (gridPos.x >= this->generatedWorld.size() || gridPos.y >= this->generatedWorld[0].size()) {
            // Resize the vectors to accommodate the grid position
            ResizeVectors(gridPos.x + 1, gridPos.y + 1);
        }

        if (this->generatedWorld[gridPos.x][gridPos.y] == nullptr) {
            auto chunkScene = CreatePlane(gridPos);
            this->newlyGeneratedWorld[gridPos.x][gridPos.y] = chunkScene;
            this->generatedWorld[gridPos.x][gridPos.y] = chunkScene;
        }
    }
}  // namespace UniverseEngine