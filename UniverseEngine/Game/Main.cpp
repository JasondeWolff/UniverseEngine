#include <UniverseEngine.h>
using namespace UniverseEngine;

int main() {
	Engine engine{};

	Handle<Model> hSusanModel = engine.GetResources().LoadModel("Assets/susanMultiple.obj");
	Model& susanModel = engine.GetResources().GetModel(hSusanModel).Value();

	Handle<Scene> hCubeModel = engine.GetResources().LoadScene("Assets/cube.usd");

	engine.Run();

	return 0;
}