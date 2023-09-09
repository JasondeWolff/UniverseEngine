#include "Resources.h"

namespace UniverseEngine {
	Resources::Resources() {
		this->models = std::make_unique<Pool<Model>>();
		this->meshes = std::make_unique<Pool<Mesh>>();
		this->materials = std::make_unique<Pool<Material>>();
		this->textures = std::make_unique<AtomicPool<Texture>>();
	}

	Handle<Model> Resources::LoadModel(const char* filePath) {
		std::string fileExtension = FileExtension(filePath);
		if (fileExtension == "usd") return LoadUSD(filePath);
		if (fileExtension == "obj") return LoadOBJ(filePath);
		return Handle<Model>::Invalid();
	}

	std::string Resources::FileExtension(const char* filePath) {
		std::string file(filePath);
		return file.substr(file.find_last_of(".") + 1);
	}
}