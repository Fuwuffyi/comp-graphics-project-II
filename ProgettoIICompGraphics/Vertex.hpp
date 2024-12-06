#pragma once

#include <glm/glm.hpp>

/**
 * Struct holding information for a single three dimensional vertex.
 */
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};