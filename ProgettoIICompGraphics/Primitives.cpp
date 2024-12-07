#include "Primitives.hpp"

#include "Mesh.hpp"
#include "Vertex.hpp"

#include <glad/glad.h>
#include <glm/gtc/constants.hpp>

Mesh* Primitives::generatePlane(const glm::vec2& dimensions, const uint32_t resolution) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    const float halfWidth = dimensions.x / 2.0f;
    const float halfHeight = dimensions.y / 2.0f;
    const float dx = dimensions.x / resolution;
    const float dy = dimensions.y / resolution;
    // Generate vertices
    for (uint32_t y = 0; y <= resolution; ++y) {
        for (uint32_t x = 0; x <= resolution; ++x) {
            const glm::vec3 position(
                -halfWidth + x * dx,
                0.0f,
                -halfHeight + y * dy
            );
            const glm::vec3 normal(0.0f, 1.0f, 0.0f);
            const glm::vec2 uv(float(x) / resolution, float(y) / resolution);
            vertices.emplace_back(Vertex { position, normal, uv });
        }
    }
    // Generate indices
    for (uint32_t y = 0; y < resolution; ++y) {
        for (uint32_t x = 0; x < resolution; ++x) {
            const uint32_t topLeft = y * (resolution + 1) + x;
            const uint32_t topRight = topLeft + 1;
            const uint32_t bottomLeft = (y + 1) * (resolution + 1) + x;
            const uint32_t bottomRight = bottomLeft + 1;
            indices.emplace_back(topLeft);
            indices.emplace_back(bottomLeft);
            indices.emplace_back(topRight);
            indices.emplace_back(topRight);
            indices.emplace_back(bottomLeft);
            indices.emplace_back(bottomRight);
        }
    }
    return new Mesh(vertices, indices, GL_TRIANGLES);
}

Mesh* Primitives::generateRect(const glm::vec3& dimensions, const uint32_t resolution) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    // TODO: Implement (FLAT SHADED)
	return new Mesh(vertices, indices, GL_TRIANGLES);
}

Mesh* Primitives::generatePyramid(const glm::vec3& dimensions, const uint32_t resolution) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	// TODO: Implement (FLAT SHADED)
	return new Mesh(vertices, indices, GL_TRIANGLES);
}

Mesh* Primitives::generateSphere(const glm::vec3& dimensions, const uint32_t resolution) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    const glm::vec3 radii = dimensions * 0.5f;
    for (uint32_t lat = 0; lat <= resolution; ++lat) {
        const float theta = glm::pi<float>() * lat / resolution;
        const float sinTheta = sin(theta);
        const float cosTheta = cos(theta);
        for (uint32_t lon = 0; lon <= resolution; ++lon) {
            const float phi = 2.0f * glm::pi<float>() * lon / resolution;
            const float sinPhi = sin(phi);
            const float cosPhi = cos(phi);
            const glm::vec3 normal = glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
            const glm::vec3 position = normal * radii;
            const glm::vec2 uv(float(lon) / resolution, float(lat) / resolution);
            vertices.emplace_back(Vertex { position, normal, uv });
        }
    }
    for (uint32_t lat = 0; lat < resolution; ++lat) {
        for (uint32_t lon = 0; lon < resolution; ++lon) {
            const uint32_t a = lat * (resolution + 1) + lon;
            const uint32_t b = a + resolution + 1;
            indices.emplace_back(a);
            indices.emplace_back(a + 1);
            indices.emplace_back(b);
            indices.emplace_back(b);
            indices.emplace_back(a + 1);
            indices.emplace_back(b + 1);
        }
    }
    return new Mesh(vertices, indices, GL_TRIANGLES);
}

Mesh* Primitives::generateCylinder(const glm::vec3& dimensions, const uint32_t resolution) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	// TODO: Implement (FLAT SHADED ON BOTTOM AND TOP, SMOOTH SHADED ON HORIZONTAL?)
	return new Mesh(vertices, indices, GL_TRIANGLES);
}

Mesh* Primitives::generateCone(const glm::vec3& dimensions, const uint32_t resolution) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    // TODO: Implement (FLAT SHADED ON BOTTOM, SMOOTH SHADED ELSEWISE?)
	return new Mesh(vertices, indices, GL_TRIANGLES);
}

Mesh* Primitives::generateThorus() {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    // TODO: Implement (SMOOTH SHADED)
	return new Mesh(vertices, indices, GL_TRIANGLES);
}
