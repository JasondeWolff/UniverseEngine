#include "TerrianGenerator.h"
#include "Engine.h"
#include "Resources.h"

namespace UniverseEngine {
    TerrianGenerator::TerrianGenerator() {
        //  Waarom heb ik geen toegang tot LoadGLTF??????
        //Handle<Scene> voxelData = Engine::GetResources().LoadGLTF("Assets/cube.gltf");
        //-----------------------------------------------------------------------------------
    }

    void TerrianGenerator::GenerateRandomChunk() {
        randomChunk.setPostion(glm::vec3(0, 0, 0));

        for (int x = 0; x < xChunkSize; x++) {
            for (int y = 0; y < yChunkSize; y++) {
                for (int z = 0; z < zChunkSize; z++) {
                    randomChunk.setVoxel(x, y, z);
                }
            }
        }
    }

    void TerrianGenerator::Update() {
        
    }
}  // namespace UniverseEngine