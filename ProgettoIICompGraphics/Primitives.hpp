#pragma once

#include <glm/glm.hpp>

class Mesh;

namespace Primitives {
	Mesh* generatePlane(const glm::vec2& dimensions = glm::vec2(1.0f), const uint32_t resolution = 1);
	Mesh* generateRect(const glm::vec3& dimensions = glm::vec3(1.0f), const uint32_t resolution = 1);
	Mesh* generatePyramid(const glm::vec3& dimensions = glm::vec3(1.0f), const uint32_t resolution = 1);
	Mesh* generateSphere(const glm::vec3& dimensions = glm::vec3(1.0f), const uint32_t resolution = 3);
	Mesh* generateCylinder(const glm::vec3& dimensions = glm::vec3(1.0f), const uint32_t resolution = 3);
	Mesh* generateCone(const glm::vec3& dimensions = glm::vec3(1.0f), const uint32_t resolution = 3);
	Mesh* generateThorus();
}
