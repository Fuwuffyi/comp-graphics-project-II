#include "BoundingBox.hpp"

#include "Vertex.hpp"

#include <cmath>
#include <limits>

static constexpr float EPSILON = 0.01f;

BoundingBox::BoundingBox(const std::vector<Vertex>& vertices)
	:
	maxMinValues({
		std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), 
		std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()
	})
{
	for (const Vertex& v : vertices) {
		this->maxMinValues[0] = std::min(maxMinValues[0], v.position.x - EPSILON);	// 0 = minX
		this->maxMinValues[1] = std::max(maxMinValues[1], v.position.x + EPSILON);	// 1 = maxX
		this->maxMinValues[2] = std::min(maxMinValues[2], v.position.y - EPSILON);	// 2 = minY
		this->maxMinValues[3] = std::max(maxMinValues[3], v.position.y + EPSILON);	// 3 = maxY
		this->maxMinValues[4] = std::max(maxMinValues[4], v.position.z - EPSILON);	// 4 = minZ
		this->maxMinValues[5] = std::max(maxMinValues[5], v.position.z + EPSILON);	// 5 = maxZ
	}
}

BoundingBox BoundingBox::transform(const glm::mat4& transformationMatrix) const {
	std::vector<Vertex> verts(8);
	// Transform the current bounding box's vertices by the matrix
	const glm::vec4 transformed0 = transformationMatrix * glm::vec4(this->maxMinValues[0], this->maxMinValues[2], this->maxMinValues[4], 1.0f);
	const glm::vec4 transformed1 = transformationMatrix * glm::vec4(this->maxMinValues[1], this->maxMinValues[2], this->maxMinValues[4], 1.0f);
	const glm::vec4 transformed2 = transformationMatrix * glm::vec4(this->maxMinValues[0], this->maxMinValues[3], this->maxMinValues[4], 1.0f);
	const glm::vec4 transformed3 = transformationMatrix * glm::vec4(this->maxMinValues[1], this->maxMinValues[3], this->maxMinValues[4], 1.0f);
	const glm::vec4 transformed4 = transformationMatrix * glm::vec4(this->maxMinValues[0], this->maxMinValues[2], this->maxMinValues[5], 1.0f);
	const glm::vec4 transformed5 = transformationMatrix * glm::vec4(this->maxMinValues[1], this->maxMinValues[2], this->maxMinValues[5], 1.0f);
	const glm::vec4 transformed6 = transformationMatrix * glm::vec4(this->maxMinValues[0], this->maxMinValues[3], this->maxMinValues[5], 1.0f);
	const glm::vec4 transformed7 = transformationMatrix * glm::vec4(this->maxMinValues[1], this->maxMinValues[3], this->maxMinValues[5], 1.0f);
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
	const bool overlapX = this->maxMinValues[1] >= other.maxMinValues[0] && other.maxMinValues[1] >= this->maxMinValues[0];
	const bool overlapY = this->maxMinValues[3] >= other.maxMinValues[2] && other.maxMinValues[3] >= this->maxMinValues[2];
	const bool overlapZ = this->maxMinValues[5] >= other.maxMinValues[4] && other.maxMinValues[5] >= this->maxMinValues[4];
	return overlapX && overlapY && overlapZ;
}