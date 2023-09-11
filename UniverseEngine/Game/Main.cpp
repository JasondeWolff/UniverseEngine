#include <UniverseEngine.h>
using namespace UniverseEngine;

int main() {
	Engine engine{};

	Handle<Scene> hSusanModel = engine.GetResources().LoadScene("Assets/susanMultiple.obj");
	Scene& susanModel = engine.GetResources().GetScene(hSusanModel).Value();

	Handle<Scene> hCubeModel = engine.GetResources().LoadScene("Assets/cube.usd");

	engine.Run();

	return 0;
}