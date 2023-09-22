#pragma once

#include "Chunk.h"

namespace UniverseEngine {
    class Resources;

    class TerrianGenerator {
    public:
        TerrianGenerator();
        void GenerateRandomChunk();
        //Scene ConvertChunkToScene();

    private:
        friend class Resources;

        Chunk randomChunk;

        void Update();
    };
}  // namespace UniverseEngine