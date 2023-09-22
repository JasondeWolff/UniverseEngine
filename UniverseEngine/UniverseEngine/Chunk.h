#pragma once
#include "Voxel.h"

namespace UniverseEngine {
    static const int xChunkSize = 16;
    static const int yChunkSize = 32;
    static const int zChunkSize = 16;

    class Chunk {
    public:
        Chunk();
        void setVoxel(int x, int y, int z);
        void setPostion(glm::vec3 position);

    private:
        glm::vec3 position;
        std::vector<std::vector<std::vector<Voxel>>> chunkData;
    };

}  // namespace UniverseEngine
