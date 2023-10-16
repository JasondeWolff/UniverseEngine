#include <array>
#include "TerrianDemo.h"

using namespace UniverseEngine;

void TerrianDemo::OnStart() {
	Resources& resources = Engine::GetResources();
	World& world = Engine::GetWorld();
	Graphics& graphics = Engine::GetGraphics();

	graphics.UseLODs(false);
	graphics.SetPolygonMode(GraphicsPolygonMode::LINE);

	std::array<std::string, 6> skyboxTexturePaths = {
		"Assets/Skyboxes/BlueSkye/right.jpg",
		"Assets/Skyboxes/BlueSkye/left.jpg",
		"Assets/Skyboxes/BlueSkye/top.jpg",
		"Assets/Skyboxes/BlueSkye/bottom.jpg",
		"Assets/Skyboxes/BlueSkye/front.jpg",
		"Assets/Skyboxes/BlueSkye/back.jpg",
	};
	std::array<std::shared_ptr<Texture>, 6> skyboxTextures;
	for (size_t i = 0; i < skyboxTexturePaths.size(); i++) {
		skyboxTextures[i] = resources.LoadTexture(skyboxTexturePaths[i], TextureType::SRGB);
	}
	//graphics.SetSkybox(skyboxTextures);

	this->camera.Start();

	int chunkWidth = 25;
	int chunkHeight = 25;
	int chunkWidthSegments = 25;
	int chunkHeightSegments = 25;
	int chunkRenderDistance = 1;

	TerrianGeneratorConfig config = {
		chunkWidth,
		chunkHeight,
		chunkWidthSegments,
		chunkHeightSegments,
		chunkRenderDistance
	};

	Engine::GetWorld().GenerateWorld(config);

    float xOffset = ((chunkRenderDistance * chunkWidth) + chunkWidth) / 2.0f;
    float zOffset = ((chunkRenderDistance * chunkHeight) + chunkHeight) / 2.0f;
    auto& camera = Engine::GetWorld().camera;
    camera.transform.Translate(glm::vec3(xOffset, -5.0f, zOffset));

	int debug = 0;
}

void TerrianDemo::Update(float deltaTime) {
	if (Engine::GetInput().GetKey(KeyCode::ESCAPE)) {
		Engine::Quit();
	}

	this->camera.Update(deltaTime);
}

void TerrianDemo::OnClose() {

}