#include "Engine.h"

#include "Logging.h"

namespace UniverseEngine {
	Engine* Engine::gInstance = nullptr;

	Engine::Engine() {
		UE_ASSERT_MSG(Engine::gInstance == nullptr, "Already an engine instance alive.");
		Engine::gInstance = this;

		this->resources = std::move(std::unique_ptr<Resources>(new Resources{}));
	}

	Resources& Engine::GetResources() {
		UE_ASSERT_MSG(Engine::gInstance && Engine::gInstance->resources.get(), "Resources hasn't been initialized.");
		return *Engine::gInstance->resources.get();
	}
}