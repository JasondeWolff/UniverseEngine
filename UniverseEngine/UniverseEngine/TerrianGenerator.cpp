#include "TerrianGenerator.h"

#include "Engine.h"
#include "Resources.h"
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"

#include <iostream>
#include <string>

#include "Engine.h"

namespace UniverseEngine {
    // Utils
    glm::vec4 HexToVec4(unsigned int hexValue) {
        float red = ((hexValue >> 16) & 0xFF) / 255.0f;
        float green = ((hexValue >> 8) & 0xFF) / 255.0f;
        float blue = (hexValue & 0xFF) / 255.0f;

        return glm::vec4(red, green, blue, 1.0f);
    }

    void TerrianGenerator::Init(TerrianGeneratorConfig config) {
        this->chunk_width = config.chunkWidth;
        this->chunk_height = config.chunkHeight;
        this->chunk_widthSegments = config.chunkWidthSegments;
        this->chunk_heightSegments = config.chunkHeightSegments;
        this->chunk_renderDistance = config.chunkRenderDistance;
        this->previousGridPos = glm::vec2(0, 0);
        this->noiseSeed = rand();
    }

    std::shared_ptr<Scene> TerrianGenerator::CreateTerrian(int gridX, int gridY) {
        auto plane = CreatePlane(gridX, gridY);
        float* heightMap = GenerateNoise(gridX, gridY);

        for (int y = 0; y <= chunk_heightSegments + 1; y++) {
            for (int x = 0; x <= chunk_widthSegments; x++) {
                int index = (y * chunk_widthSegments) + x;
                auto vertex = &plane->vertices[index];

                if (heightMap[index] < 0)
                    heightMap[index] = 0;

                float heightValue = heightMap[index] * heightMap[index] * 50.0f;

                vertex->position.y = heightValue;

                if (heightMap[index] == 0) {
                    vertex->color = HexToVec4(climate::Water);
                } 
                else if (heightMap[index] <= 0.25f) {
                    vertex->color = HexToVec4(climate::Sand);
                } 
                else if (heightMap[index] <= 0.50f) {
                    vertex->color = HexToVec4(climate::Grass);
                } 
                else if (heightMap[index] <= 0.48f) {
                    vertex->color = HexToVec4(climate::Woods);
                } 
                else if (heightMap[index] <= 0.65f) {
                    vertex->color = HexToVec4(climate::Rocks);
                } 
                else{
                    vertex->color = HexToVec4(climate::Snow);
                }
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
            0.01f,
            noiseSeed
        );

        return noiseData;
    }

    void TerrianGenerator::GenerateNewTerrain(int gridPosX, int gridPosY) 
    {
        auto hScene = CreateTerrian(gridPosX, gridPosY);

        auto sceneInstance = Engine::GetWorld().AddSceneInstance(hScene);

        glm::vec3 position = glm::vec3((gridPosX * chunk_width) + chunk_width, -30.0f, (gridPosY * chunk_height) + chunk_height);
        Transform chunkTrans = Transform();
        chunkTrans.SetTranslation(position);
        sceneInstance->transform = chunkTrans;

        terrian.emplace_back(sceneInstance);
    }

    void TerrianGenerator::Update() {
        auto camera = Engine::GetWorld().camera;
        const glm::vec3 cameraPos = camera.transform.GetTranslation();

        const glm::ivec2 gridPos = {
            static_cast<int>((cameraPos.x - chunk_width) / chunk_width),
            static_cast<int>((cameraPos.z - chunk_height) / chunk_height)
        };
        
        bool open = true;
        ImGui::SetNextWindowSize(ImVec2(200, 250));
        ImGui::Begin("[TERRIAN] General", &open);
        ImGui::Text("GridPos: %dx%d \n", gridPos.x, gridPos.y);
        ImGui::End();

         
        // Define the range of grid positions to generate terrain around the player
        int gridRange = chunk_renderDistance;  // Adjust this as needed

        for (int i = -gridRange; i <= gridRange; i++) {
            for (int j = -gridRange; j <= gridRange; j++) {
                glm::ivec2 currentGrid = {gridPos.x + i, gridPos.y + j};

                // Check if terrain needs to be generated for this grid position
                if (currentGrid != previousGridPos) {
                    if (loadedChunks.find(currentGrid) == loadedChunks.end()) {
                        // Generate terrain only if it's not already loaded
                        GenerateNewTerrain(currentGrid.x, currentGrid.y);
                        loadedChunks.insert(currentGrid);
                    }
                    previousGridPos = currentGrid;
                }
            }
        }
    }
}  // namespace UniverseEngine