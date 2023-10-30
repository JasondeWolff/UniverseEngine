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
        this->previousGridPos = glm::vec2(0, 0);

        for (int y = 0; y < chunk_renderDistance; y++) {
            for (int x = 0; x < chunk_renderDistance; x++) {

                auto hScene = CreateTerrian(x, y);

                auto sceneInstance = Engine::GetWorld().AddSceneInstance(hScene);

                glm::vec3 position = glm::vec3(
                    (x * chunk_width) + chunk_width, 
                    -30.0f, 
                    (y * chunk_height) + chunk_height
                );
                Transform chunkTrans = Transform();
                chunkTrans.SetTranslation(position);
                sceneInstance->transform = chunkTrans;

                terrian.emplace_back(sceneInstance);
            }
        }
    }

    std::shared_ptr<Scene> TerrianGenerator::CreateTerrian(int x, int y) {
        auto plane = CreatePlane(x, y);
        float* heightMap = GenerateNoise(x, y);

        for (int y = 0; y <= chunk_heightSegments + 1; y++) {
            for (int x = 0; x <= chunk_widthSegments; x++) {
                int index = (y * chunk_widthSegments) + x;
                auto vertex = &plane->vertices[index];

                if (heightMap[index] < 0)
                    heightMap[index] = 0;

                float heightValue = heightMap[index] * heightMap[index] * 50.0f;

                vertex->position.y = heightValue;
            }
        }

        auto hScene = Engine::GetResources().CreateScene(std::move(plane->BuildFlatShaded()));
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
        for (int y = 0; y <= chunk_heightSegments; ++y) {
            for (int x = 0; x <= chunk_widthSegments; ++x) {
                Vertex vertex;

                vertex.position = glm::vec3(x * dx - chunk_width / 2.0f, 1.0f,
                                            y * dy - chunk_height / 2.0f);
                vertex.texCoord =
                    glm::vec2(static_cast<float>(x) / static_cast<float>(chunk_widthSegments),
                              static_cast<float>(y) / static_cast<float>(chunk_heightSegments));

                auto randColor = rand() % 255;
                vertex.color = glm::vec4(randColor, randColor, randColor, 1.0f);

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
        fnFractal->SetOctaveCount(7);

        auto width = chunk_widthSegments + 1;
        auto height = chunk_heightSegments + 1;
        glm::ivec2 offset = glm::vec2(x * chunk_widthSegments, y * chunk_heightSegments);

        float* noiseData = new float[width * height]();
        fnFractal->GenUniformGrid2D(
            noiseData, 
            offset.x, 
            offset.y, 
            width, 
            height, 
            0.005f,
            1337
        );

        return noiseData;
    }

    void TerrianGenerator::GenerateNewTerrian(int gridPosX, int gridPosY) 
    {
        //Generate some more terrian
        printf("GridPos: %dx%d \n", gridPosX, gridPosY);

        auto hScene = CreateTerrian(gridPosX, gridPosY);

        auto sceneInstance = Engine::GetWorld().AddSceneInstance(hScene);

        glm::vec3 position = glm::vec3((gridPosX * chunk_width) + chunk_width, -30.0f,
                                        (gridPosY * chunk_height) + chunk_height);
        Transform chunkTrans = Transform();
        chunkTrans.SetTranslation(position);
        sceneInstance->transform = chunkTrans;

        terrian.emplace_back(sceneInstance);
    }

    void TerrianGenerator::Update() {
        auto camera = Engine::GetWorld().camera;
        const glm::vec3 cameraPos = camera.transform.GetTranslation();

        float xOffset = (float)(chunk_width * chunk_renderDistance) + chunk_width;
        float zOffset = (float)(chunk_height * chunk_renderDistance) + chunk_height;
        xOffset = xOffset / 2.0f;
        zOffset = zOffset / 2.0f;

        const glm::ivec2 gridPos = {
            static_cast<int>((cameraPos.x - xOffset) / chunk_width),
            static_cast<int>((cameraPos.z - zOffset) / chunk_height)
        };
        
        bool open = true;
        ImGui::SetNextWindowSize(ImVec2(200, 250));
        ImGui::Begin("[TERRIAN] General", &open);
        ImGui::Text("GridPos: %dx%d \n", gridPos.x, gridPos.y);
        ImGui::End();

        if (gridPos != previousGridPos) {
            //GenerateNewTerrian(gridPos.x, gridPos.y);
            previousGridPos = gridPos;
        }
    }
}  // namespace UniverseEngine