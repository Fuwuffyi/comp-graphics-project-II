#pragma once

#include <glm/glm.hpp>

class Mesh;

namespace Primitives {
	Mesh* generatePlane(const uint32_t resolution = 1);
	Mesh* generateCube(const uint32_t resolution = 1);
	Mesh* generatePyramid(const uint32_t resolution);
	Mesh* generateSphere(const uint32_t resolution = 3);
	Mesh* generateCylinder(const float bottomRadius = 1.0f, const float topRadius = 1.0f, const float length = 1.0f, const int slices = 5, const int stacks = 5);
	Mesh* generateCone(const float radius = 1.0f, const float length = 1.0f, const int slices = 5, const int stacks = 5);
	Mesh* generateThorus(const float innerRadius = 1.0f, const float circleRadius = 0.25f, const uint32_t resolution = 5);
}
