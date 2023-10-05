#include <UniverseEngine.h>

#include "MyGame.h"
#include "AssetDemo.h"
#include "LODDemo.h"
#include "CloudDemo.h"

int main() {
	UniverseEngine::Engine engine{};
	engine.Start<CloudDemo>();

	return 0;
}