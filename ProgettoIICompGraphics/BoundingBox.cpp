#include "BoundingBox.hpp"

#include "Vertex.hpp"

#include <cmath>
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
	verts[0] = Vertex{ glm::vec3(transformed0.x, transformed0.y, transformed0.z), glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[1] = Vertex{ glm::vec3(transformed1.x, transformed1.y, transformed1.z), glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[2] = Vertex{ glm::vec3(transformed2.x, transformed2.y, transformed2.z), glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[3] = Vertex{ glm::vec3(transformed3.x, transformed3.y, transformed3.z), glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[4] = Vertex{ glm::vec3(transformed4.x, transformed4.y, transformed4.z), glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[5] = Vertex{ glm::vec3(transformed5.x, transformed5.y, transformed5.z), glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[6] = Vertex{ glm::vec3(transformed6.x, transformed6.y, transformed6.z), glm::vec3(0.0f), glm::vec2(0.0f) };
	verts[7] = Vertex{ glm::vec3(transformed7.x, transformed7.y, transformed7.z), glm::vec3(0.0f), glm::vec2(0.0f) };
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
