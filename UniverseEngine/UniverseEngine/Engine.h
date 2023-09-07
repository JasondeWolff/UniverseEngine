#pragma once

namespace UniverseEngine {
	class Engine {
	public:
		Engine(unsigned width, unsigned height);

	private:
		static Engine* gInstance;
	};
}