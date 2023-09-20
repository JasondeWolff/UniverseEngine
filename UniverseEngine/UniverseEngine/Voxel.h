#pragma once
#include "Mesh.h"

namespace UniverseEngine {
    enum VoxelType {
        Water,
        Land,
    };

    class Voxel : Mesh {
    public:
        Voxel(int width = 1, int height = 1);
        ~Voxel();

    private:
        int m_Width;
        int m_Height;
        VoxelType type;

    };
}  // namespace UniverseEngine
