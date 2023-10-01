#include <UniverseEngine.h>

#include "MyGame.h"
#include "AssetDemo.h"
#include "LODDemo.h"

int main() {
	UniverseEngine::Engine engine{};
	engine.Start<LODDemo>();

	return 0;
}