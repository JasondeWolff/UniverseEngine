#pragma once

#include <glm/vec4.hpp>

#include "Texture.h"
#include "Pool.h"

namespace UniverseEngine {
	struct Material {
		glm::vec4 baseColor;
		Handle<Texture> baseColorMap;
	};
}