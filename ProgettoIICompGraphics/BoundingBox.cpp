#include "BoundingBox.hpp"

#include "Vertex.hpp"
#include <limits>

static constexpr float EPSILON = 0.01f;

BoundingBox::BoundingBox(const std::vector<Vertex>& vertices)
	:
	maxValues(-std::numeric_limits<float>::infinity()),
	minValues(std::numeric_limits<float>::infinity())
{
	for (const Vertex& v : vertices) {
		this->maxValues = glm::max(this->maxValues, v.position + EPSILON);
		this->minValues = glm::min(this->minValues, v.position - EPSILON);
	}
}

BoundingBox BoundingBox::transform(const glm::mat4& transformationMatrix) const {
	std::vector<Vertex> verts(8);
	// Transform the current bounding box's vertices by the matrix
	const glm::vec4 transformed0 = transformationMatrix * glm::vec4(this->minValues.x, this->minValues.y, this->minValues.z, 1.0f);
	const glm::vec4 transformed1 = transformationMatrix * glm::vec4(this->maxValues.x, this->minValues.y, this->minValues.z, 1.0f);
	const glm::vec4 transformed2 = transformationMatrix * glm::vec4(this->minValues.x, this->maxValues.y, this->minValues.z, 1.0f);
	const glm::vec4 transformed3 = transformationMatrix * glm::vec4(this->maxValues.x, this->maxValues.y, this->minValues.z, 1.0f);
	const glm::vec4 transformed4 = transformationMatrix * glm::vec4(this->minValues.x, this->minValues.y, this->maxValues.z, 1.0f);
	const glm::vec4 transformed5 = transformationMatrix * glm::vec4(this->maxValues.x, this->minValues.y, this->maxValues.z, 1.0f);
	const glm::vec4 transformed6 = transformationMatrix * glm::vec4(this->minValues.x, this->maxValues.y, this->maxValues.z, 1.0f);
	const glm::vec4 transformed7 = transformationMatrix * glm::vec4(this->maxValues.x, this->maxValues.y, this->maxValues.z, 1.0f);
	verts[0] = Vertex{ transformed0, glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[1] = Vertex{ transformed1, glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[2] = Vertex{ transformed2, glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[3] = Vertex{ transformed3, glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[4] = Vertex{ transformed4, glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[5] = Vertex{ transformed5, glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[6] = Vertex{ transformed6, glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[7] = Vertex{ transformed7, glm::vec3(0.0f), glm::vec2(0.0f) };
	// Generate a new bounding box based on those vertices
	return BoundingBox(verts);
}

bool BoundingBox::checkCollisions(const BoundingBox& other) const {
	const bool overlapX = this->maxValues.x >= other.minValues.x && other.maxValues.x >= this->minValues.x;
	const bool overlapY = this->maxValues.y >= other.minValues.y && other.maxValues.y >= this->minValues.y;
	const bool overlapZ = this->maxValues.z >= other.minValues.z && other.maxValues.z >= this->minValues.z;
	return overlapX && overlapY && overlapZ;
}

bool BoundingBox::checkCollisions(const glm::vec3& point) const {
	return (point.x >= this->minValues.x && point.x <= this->maxValues.x) &&
           (point.y >= this->minValues.y && point.y <= this->maxValues.y) &&
           (point.z >= this->minValues.z && point.z <= this->maxValues.z);
}

bool BoundingBox::rayIntersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, float& tMin, float& tMax) const {
	// Early exit if ray direction is zero (no movement)
	if (glm::all(glm::equal(rayDirection, glm::vec3(0.0f)))) {
		return false; // No intersection since the ray doesn't move
	}
	// Calculate the inverse of the ray direction
	const glm::vec3 invDir = 1.0f / rayDirection;
	// Calculate intersection with the planes defined by the bounding box
	const glm::vec3 t0 = (minValues - rayOrigin) * invDir;
	const glm::vec3 t1 = (maxValues - rayOrigin) * invDir;
	// Swap t0 and t1 for negative directions
	const glm::vec3 tNear = glm::min(t0, t1);
	const glm::vec3 tFar = glm::max(t0, t1);
	// Calculate tMin and tMax (intersection distances along the ray)
	tMin = glm::max(tNear.x, glm::max(tNear.y, tNear.z));
	tMax = glm::min(tFar.x, glm::min(tFar.y, tFar.z));
	// If tMax < tMin, no intersection occurs
	return tMax > glm::max(0.0f, tMin);
}
