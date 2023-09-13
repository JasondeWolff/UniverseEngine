#include "Mesh.h"

namespace UniverseEngine {
    void Mesh::ClearCPUData() {
        this->vertices.clear();
        this->vertices.shrink_to_fit();
        this->indices.clear();
        this->indices.shrink_to_fit();
    }
}