#include "Engine.h"

#include "Logging.h"

namespace UniverseEngine {
	Engine::Engine(unsigned width, unsigned height) {
		UE_ASSERT_MSG(Engine::gInstance == nullptr, "Already an engine instance alive!");
		Engine::gInstance = this;
	}
}