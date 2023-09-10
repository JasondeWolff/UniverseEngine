#include <UniverseEngine.h>
using namespace UniverseEngine;

int main() {
	Engine engine{};

	Handle<Model> hSusanModel = engine.GetResources().LoadModel("Assets/susan.obj");
	Model& susanModel = engine.GetResources().GetModel(hSusanModel).Value();

	//Handle<Model> hCubeModel = engine.GetResources().LoadModel("Assets/cube.usd");

	return 0;
}