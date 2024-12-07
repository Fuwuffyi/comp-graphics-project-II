#include "Primitives.hpp"

#include "Mesh.hpp"
#include "Vertex.hpp"

#include <glad/glad.h>
#include <glm/gtc/constants.hpp>

Mesh* Primitives::generatePlane(const uint32_t resolution) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    const float dx = 1.0f / resolution;
    const float dy = 1.0f / resolution;
    // Generate vertices
    for (uint32_t y = 0; y <= resolution; ++y) {
        for (uint32_t x = 0; x <= resolution; ++x) {
            const glm::vec3 position(
                -0.5f + x * dx,
                0.0f,
                -0.5f + y * dy
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

Mesh* Primitives::generateCube(const uint32_t resolution) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    // TODO: Implement (FLAT SHADED)
	return new Mesh(vertices, indices, GL_TRIANGLES);
}

Mesh* Primitives::generatePyramid(const uint32_t resolution) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	// TODO: Implement (FLAT SHADED)
	return new Mesh(vertices, indices, GL_TRIANGLES);
}

Mesh* Primitives::generateSphere(const uint32_t resolution) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    for (uint32_t lat = 0; lat <= resolution; ++lat) {
        const float theta = glm::pi<float>() * lat / resolution;
        const float sinTheta = sin(theta);
        const float cosTheta = cos(theta);
        for (uint32_t lon = 0; lon <= resolution; ++lon) {
            const float phi = 2.0f * glm::pi<float>() * lon / resolution;
            const float sinPhi = sin(phi);
            const float cosPhi = cos(phi);
            const glm::vec3 normal = glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
            const glm::vec3 position = normal;
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

Mesh* Primitives::generateCylinder(const float bottomRadius, const float topRadius, const uint32_t resolution) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	// TODO: Implement (FLAT SHADED ON BOTTOM AND TOP, SMOOTH SHADED ON HORIZONTAL?)
	return new Mesh(vertices, indices, GL_TRIANGLES);
}

Mesh* Primitives::generateCone(const float radius, const uint32_t resolution) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    // TODO: Implement (FLAT SHADED ON BOTTOM, SMOOTH SHADED ELSEWISE?)
	return new Mesh(vertices, indices, GL_TRIANGLES);
}

Mesh* Primitives::generateThorus(const float innerRadius, const float circleRadius, const uint32_t resolution) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    // TODO: Implement (SMOOTH SHADED)
	return new Mesh(vertices, indices, GL_TRIANGLES);
}
