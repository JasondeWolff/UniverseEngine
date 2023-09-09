#include <UniverseEngine.h>
using namespace UniverseEngine;

int main() {
	Engine engine{};

	Handle<Model> hSusanModel = engine.GetResources().LoadModel("Assets/susan.obj");
	Model& susanModel = engine.GetResources().GetModel(hSusanModel).Value();

	return 0;
}