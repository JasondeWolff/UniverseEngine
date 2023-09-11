#include <UniverseEngine.h>
using namespace UniverseEngine;

int main() {
	Engine engine{};

	Handle<Scene> hSusan = engine.GetResources().LoadScene("Assets/susanMultiple.obj");
	Handle<Scene> hCubeModel = engine.GetResources().LoadScene("Assets/cube.usd");

	Handle<SceneInstance> hSusanInstance = engine.GetWorld().AddSceneInstance(hSusan);

	engine.Run();

	return 0;
}