#include "Chunk.h"

namespace UniverseEngine {
    Chunk::Chunk() {
        chunkData.resize(xChunkSize);
        for (size_t i = 0; i < chunkData.size(); i++) {
            chunkData.at(i).resize(yChunkSize);
            for (size_t j = 0; j < chunkData.at(i).size(); j++) {
                chunkData.at(i).at(j).resize(zChunkSize);
            }
        }

        int debug = 0;
    }
    void Chunk::setVoxel(int x, int y, int z) {

        VoxelType type = VoxelType(rand() % 2);

        chunkData.at(x).at(y).at(z).type = type;
    }

    void Chunk::setPostion(glm::vec3 position) {

    }
}  // namespace UniverseEngine
