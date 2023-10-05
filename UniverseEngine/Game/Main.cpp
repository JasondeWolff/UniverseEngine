#include <UniverseEngine.h>

#include "MyGame.h"
#include "AssetDemo.h"
#include "LODDemo.h"
#include "CloudDemo.h"
#include "TerrianDemo.h"

int main() {
	UniverseEngine::Engine engine{};
	engine.Start<TerrianDemo>();

	return 0;
}