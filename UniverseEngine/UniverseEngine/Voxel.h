#pragma once
#include "Mesh.h"

namespace UniverseEngine {
    enum VoxelType {
        None,
        Block,
    };

    /*class Voxel : Mesh {
    public:
        Voxel();
        ~Voxel(); 

    private:
        VoxelType type;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };*/

    struct Voxel {
        VoxelType type;
    };
}  // namespace UniverseEngine
