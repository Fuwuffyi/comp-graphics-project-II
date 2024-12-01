#pragma once

#include <glm/glm.hpp>

/**
 * Struct holding information for a single two dimensional vertex.
 */
struct Vertex2D {
	glm::vec2 position;
	glm::vec2 normal;
	glm::vec2 uv;
};

/**
 * Struct holding information for a single three dimensional vertex.
 */
struct Vertex3D {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};