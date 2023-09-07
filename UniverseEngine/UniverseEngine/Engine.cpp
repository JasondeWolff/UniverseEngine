#include "Engine.h"

namespace UniverseEngine {
	Engine::Engine(unsigned width, unsigned height) {
		Engine::gInstance = this;
	}
}