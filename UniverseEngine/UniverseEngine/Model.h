#pragma once

#include "Material.h"
#include "Mesh.h"

namespace UniverseEngine {
<<<<<<< HEAD
	struct Model {
		Model() {};
	};
}
=======
    struct Model {
        std::string name;

        std::vector<Mesh> meshes;
        std::vector<Material> materials;
    };
}  // namespace UniverseEngine
>>>>>>> 62e9eae5af26075071d4b555ac28f927aa2c57f3
