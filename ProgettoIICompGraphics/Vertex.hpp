#pragma once

#include <glm/glm.hpp>

/**
 * Struct holding information for a single three dimensional vertex.
 */
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal = glm::vec3(0.0f);
	glm::vec2 uv = glm::vec2(0.0f);
	glm::vec3 tangent = glm::vec3(0.0f); /* Used to transform tangent space textures to object space */
	glm::vec3 bitangent = glm::vec3(0.0f); /* Used to transform tangent space textures to object space */
};