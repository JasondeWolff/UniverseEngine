#include "Resources.h"

namespace UniverseEngine {
	Resources::Resources() {
		this->models = std::make_unique<Pool<Model>>();
		this->meshes = std::make_unique<Pool<Mesh>>();
		this->materials = std::make_unique<Pool<Material>>();
		this->textures = std::make_unique<AtomicPool<Texture>>();
	}
}