#include <UniverseEngine.h>

#include "MyGame.h"

int main() {
	UniverseEngine::Engine engine{};
	engine.Start<MyGame>();

	return 0;
}