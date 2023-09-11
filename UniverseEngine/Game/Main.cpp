#include <UniverseEngine.h>
using namespace UniverseEngine;

int main() {
	Engine engine{};

	Handle<Model> hSusanModel = engine.GetResources().LoadModel("Assets/susanMultiple.obj");
	Model& susanModel = engine.GetResources().GetModel(hSusanModel).Value();

	Handle<Model> hCubeModel = engine.GetResources().LoadModel("Assets/cube.usd");

	engine.Run();

	return 0;
}