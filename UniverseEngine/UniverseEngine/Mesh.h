#pragma once

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace UniverseEngine {
	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};

	struct Mesh {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		size_t materialIdx;
	};
}