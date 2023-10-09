#include "TerrianGenerator.h"

#include "Engine.h"
#include "Resources.h"
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"

#include <iostream>
#include <string>
#include <vector>

#include "Engine.h"

namespace UniverseEngine {
    void TerrianGenerator::Init(TerrianGeneratorConfig config) {
        this->chunk_width = config.chunkWidth;
        this->chunk_height = config.chunkHeight;
        this->chunk_widthSegments = config.chunkWidthSegments;
        this->chunk_heightSegments = config.chunkHeightSegments;
        this->chunk_renderDistance = config.chunkRenderDistance;

        for (int y = 0; y < chunk_renderDistance; y++) {
            for (int x = 0; x < chunk_renderDistance; x++) {

                auto hScene = CreateScene(x, y);

                auto sceneInstance = Engine::GetWorld().AddSceneInstance(hScene);

                glm::vec3 position = glm::vec3((x * chunk_width) + chunk_width, -30.0f, (y * chunk_height) + chunk_height);
                Transform chunkTrans = Transform();
                chunkTrans.SetTranslation(position);
                sceneInstance->transform = chunkTrans;
            }
        }
    }

    std::shared_ptr<Scene> TerrianGenerator::CreateScene(int x, int y) {
        auto plane = CreatePlane(x, y);
        float* heightMap = GenerateNoise(x, y);

        for (int y = 0; y <= chunk_heightSegments + 1; ++y) {
            for (int x = 0; x <= chunk_widthSegments; ++x) {
                auto vertex = &plane->vertices[(y * chunk_widthSegments) + x];

                if (heightMap[(y * chunk_widthSegments) + x] < 0)
                    heightMap[(y * chunk_widthSegments) + x] = 0;

                float heightValue = 
                    heightMap[(y * chunk_widthSegments) + x] * 5.f;

                vertex->position.y = heightValue;
            }
        }
        plane->GenerateNormals();

        auto hScene = Engine::GetResources().CreateScene(std::move(*plane));
        return hScene;
    }

    Mesh* TerrianGenerator::CreatePlane(int x, int y) {
        // Calculate step sizes
        float dx = chunk_width / static_cast<float>(chunk_widthSegments);
        float dy = chunk_height / static_cast<float>(chunk_heightSegments);

        Mesh* chunk = new Mesh();
        chunk->name = "Plane coord: " + std::to_string(x) + ", " + std::to_string(y);
        chunk->vertices.clear();
        chunk->vertices.reserve(chunk_widthSegments * chunk_heightSegments);

        // Generate vertices
        for (int y = 0; y <= chunk_widthSegments; ++y) {
            for (int x = 0; x <= chunk_heightSegments; ++x) {
                Vertex vertex;

                vertex.position = glm::vec3(x * dx - chunk_width / 2.0f, 1.0f,
                                            y * dy - chunk_height / 2.0f);
                vertex.texCoord =
                    glm::vec2(static_cast<float>(x) / static_cast<float>(chunk_widthSegments),
                              static_cast<float>(y) / static_cast<float>(chunk_heightSegments));
                chunk->vertices.push_back(vertex);
            }
        }

        // Generate indices
        for (int y = 0; y < chunk_heightSegments; ++y) {
            for (int x = 0; x < chunk_widthSegments; ++x) {
                int vertexIndex = y * (chunk_widthSegments + 1) + x;
                chunk->indices.push_back(vertexIndex + chunk_widthSegments + 1);
                chunk->indices.push_back(vertexIndex + 1);
                chunk->indices.push_back(vertexIndex);
                
                chunk->indices.push_back(vertexIndex + chunk_widthSegments + 1);
                chunk->indices.push_back(vertexIndex + chunk_widthSegments + 2);
                chunk->indices.push_back(vertexIndex + 1);
            }
        }
        chunk->materialIdx = 0;
        
        return chunk;
    }

    float* TerrianGenerator::GenerateNoise(int x, int y) {
        auto fnSimplex = FastNoise::New<FastNoise::Simplex>();
        auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();
        fnFractal->SetSource(fnSimplex);
        fnFractal->SetOctaveCount(5);

        auto width = chunk_widthSegments + 1;
        auto height = chunk_heightSegments + 1;
        glm::ivec2 offset = glm::vec2(x * chunk_widthSegments - 1, y * chunk_heightSegments - 1);

        float* noiseData = new float[width * height]();
        fnFractal->GenUniformGrid2D(
            noiseData, 
            offset.x, 
            offset.y, 
            width, 
            height, 
            0.05f,
            1337
        );


        return noiseData;
    }

    void TerrianGenerator::Update() {
        auto camera = Engine::GetWorld().camera;
        glm::vec3 cameraPos = camera.transform.GetTranslation();

        const glm::ivec2 gridPos = {
            static_cast<int>(cameraPos.x) / chunk_width,
            static_cast<int>(cameraPos.y) / chunk_height
        };

    }
}  // namespace UniverseEngine