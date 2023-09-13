#include <UniverseEngine.h>
using namespace UniverseEngine;

int main() {
	Engine engine{};

	Handle<Scene> hSusan = engine.GetResources().LoadScene("Assets/susanMultiple.obj");
	//Handle<Scene> hCubeModel = engine.GetResources().LoadScene("Assets/home_flower.usd");

	Handle<SceneInstance> hSusanInstance = engine.GetWorld().AddSceneInstance(hSusan);

	engine.Run();

	return 0;
}