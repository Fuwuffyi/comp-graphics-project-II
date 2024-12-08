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
    // Calculate side length based on resolution
    const float sideLength = 1.0f / resolution;
    // Generate vertices for each face
    for (int32_t face = 0; face < 6; ++face) {
        for (uint32_t y = 0; y <= resolution; ++y) {
            for (uint32_t x = 0; x <= resolution; ++x) {
                glm::vec3 position;
                glm::vec3 normal;
                glm::vec2 uv;
                // Determine face orientation and calculate vertex attributes
                switch (face) {
                    // Front face
                    case 0:
                        position = glm::vec3(-0.5f + x * sideLength, -0.5f + y * sideLength, 0.5f);
                        normal = glm::vec3(0.0f, 0.0f, 1.0f);
                        uv = glm::vec2(x / resolution, y / resolution);
                        break;
                    // Back face
                    case 1:
                        position = glm::vec3(0.5f - x * sideLength, -0.5f + y * sideLength, -0.5f);
                        normal = glm::vec3(0.0f, 0.0f, -1.0f);
                        uv = glm::vec2(1.0f - x / resolution, y / resolution);
                        break;
                    // Left face
                    case 2:
                        position = glm::vec3(-0.5f, -0.5f + y * sideLength, -0.5f + x * sideLength);
                        normal = glm::vec3(-1.0f, 0.0f, 0.0f);
                        uv = glm::vec2(1.0f - y / resolution, x / resolution);
                        break;
                    // Right face
                    case 3:
                        position = glm::vec3(0.5f, -0.5f + y * sideLength, 0.5f - x * sideLength);
                        normal = glm::vec3(1.0f, 0.0f, 0.0f);
                        uv = glm::vec2(y / resolution, x / resolution);
                        break;
                    // Top face
                    case 4:
                        position = glm::vec3(-0.5f + x * sideLength, 0.5f, -0.5f + y * sideLength);
                        normal = glm::vec3(0.0f, 1.0f, 0.0f);
                        uv = glm::vec2(x / resolution, 1.0f - y / resolution);
                        break;
                    // Bottom face
                    case 5:
                        position = glm::vec3(-0.5f + x * sideLength, -0.5f, -0.5f + y * sideLength);
                        normal = glm::vec3(0.0f, -1.0f, 0.0f);
                        uv = glm::vec2(x / resolution, y / resolution);
                        break;
                }
                vertices.emplace_back(Vertex{ position, normal, uv });
            }
        }
    }
    // Generate indices for each face
    for (int32_t face = 0; face < 6; ++face) {
        for (uint32_t y = 0; y < resolution; ++y) {
            for (uint32_t x = 0; x < resolution; ++x) {
                const uint32_t offset = face * (resolution + 1) * (resolution + 1);
                const uint32_t topLeft = offset + y * (resolution + 1) + x;
                const uint32_t topRight = topLeft + 1;
                const uint32_t bottomLeft = offset + (y + 1) * (resolution + 1) + x;
                const uint32_t bottomRight = bottomLeft + 1;
                if (face == 4) {
                    // Inverse the winding order to make the top face counter-clockwise
                    indices.emplace_back(topLeft);
                    indices.emplace_back(bottomLeft);
                    indices.emplace_back(topRight);
                    indices.emplace_back(topRight);
                    indices.emplace_back(bottomLeft);
                    indices.emplace_back(bottomRight);
                } else {
                    indices.emplace_back(topLeft);
                    indices.emplace_back(topRight);
                    indices.emplace_back(bottomLeft);
                    indices.emplace_back(topRight);
                    indices.emplace_back(bottomRight);
                    indices.emplace_back(bottomLeft);
                }
            }
        }
    }
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

Mesh* Primitives::generateCylinder(const float bottomRadius, const float topRadius, const float length, const int slices, const int stacks) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    // Calculate step sizes for slicing and stacking
    const float sliceStep = glm::pi<float>() * 2.0f / slices;
    const float heightStep = length / stacks;
    const float radiusStep = (topRadius - bottomRadius) / stacks;
    // Start at the bottom of the cylinder
    vertices.emplace_back(Vertex{ glm::vec3(0.0f, -length / 2.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.5f, 0.5f) });
    // Vertices for the sides and the top
    float currentHeight = -length / 2.0f;
    float currentRadius = bottomRadius;
    // For each stack layer
    for (int32_t i = 0; i <= stacks; i++) {
        float sliceAngle = 0.0f;
        for (int32_t j = 0; j < slices; j++) {
            // Calculate vertex position for current slice
            const float x = currentRadius * cos(sliceAngle);
            const float y = currentHeight;
            const float z = currentRadius * sin(sliceAngle);
            // Create a vertex and add it to the vertex list
            vertices.emplace_back(Vertex{ glm::vec3(x, y, z), glm::normalize(glm::vec3(x, y, z)), glm::vec2(static_cast<float>(j) / slices, static_cast<float>(i) / stacks) });
            sliceAngle += sliceStep;
        }
        // Move to the next height level and radius
        currentHeight += heightStep;
        currentRadius += radiusStep;
    }
    // Top center vertex
    vertices.emplace_back(Vertex{ glm::vec3(0.0f, length / 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.5f) });
    uint32_t baseIndex = 1; // Starting index for the side vertices
    // Bottom face indices
    for (uint32_t i = 0; i < slices; i++) {
        indices.emplace_back(0);
        indices.emplace_back(i + 1);
        indices.emplace_back((i + 1) % slices + 1);
    }
    // Side face indices
    for (uint32_t i = 0; i < stacks; i++) {
        for (uint32_t j = 0; j < slices; j++) {
            const uint32_t current = baseIndex + i * slices + j;
            const uint32_t next = baseIndex + i * slices + (j + 1) % slices;
            const uint32_t currentTop = baseIndex + (i + 1) * slices + j;
            const uint32_t nextTop = baseIndex + (i + 1) * slices + (j + 1) % slices;
            // Two triangles per quad
            indices.emplace_back(current);
            indices.emplace_back(currentTop);
            indices.emplace_back(next);
            indices.emplace_back(next);
            indices.emplace_back(currentTop);
            indices.emplace_back(nextTop);
        }
    }
    // Top face indices
    baseIndex = vertices.size() - slices - 1;
    for (uint32_t i = 0; i < slices; i++) {
        indices.emplace_back(vertices.size() - 1);
        indices.emplace_back(baseIndex + (i + 1) % slices);
        indices.emplace_back(baseIndex + i);
    }
    return new Mesh(vertices, indices, GL_TRIANGLES);
}

Mesh* Primitives::generateCone(const float radius, const float length, const int slices, const int stacks) {
	return Primitives::generateCylinder(radius, 0.0f, length, slices, stacks);
}

Mesh* Primitives::generateThorus(const float innerRadius, const float circleRadius, const uint32_t resolution) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    // TODO: Implement (SMOOTH SHADED)
	return new Mesh(vertices, indices, GL_TRIANGLES);
}
