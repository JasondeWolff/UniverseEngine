#include "Resources.h"

#include <tinyusdz/tinyusdz.hh>

namespace UniverseEngine {
	Resources::Resources() {
		this->models = std::make_unique<Pool<Model>>();
		this->meshes = std::make_unique<Pool<Mesh>>();
		this->materials = std::make_unique<Pool<Material>>();
		this->textures = std::make_unique<AtomicPool<Texture>>();

		loadUSDModel();
	}

	void Resources::loadUSDModel() 
	{
		/*tinyusdz::Stage s; 
		std::string warning;
		std::string error;

		tinyusdz::LoadUSDFromFile("assets/cube.usd", &s, &warning, &error);*/
	}
}