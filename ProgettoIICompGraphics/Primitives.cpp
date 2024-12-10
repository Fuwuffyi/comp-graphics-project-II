#include "Primitives.hpp"

#include "Mesh.hpp"
#include "Vertex.hpp"
#include <glad/glad.h>
#include <glm/gtc/constants.hpp>

std::shared_ptr<Mesh> Primitives::generatePlane(const uint32_t resolution, const glm::vec2 uvScale) {
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
            vertices.emplace_back(Vertex { position, normal, uv * uvScale });
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
    return std::make_shared<Mesh>(vertices, indices, GL_TRIANGLES);
}

std::shared_ptr<Mesh> Primitives::generateCube(const uint32_t resolution, const glm::vec2 uvScale) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    // Calculate side length based on resolution
    const float sideLength = 1.0f / resolution;
    // Generate vertices for each face
    for (uint32_t face = 0; face < 6; ++face) {
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
                vertices.emplace_back(Vertex{ position, normal, uv * uvScale });
            }
        }
    }
    // Generate indices for each face
    for (uint32_t face = 0; face < 6; ++face) {
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
    return std::make_shared<Mesh>(vertices, indices, GL_TRIANGLES);
}

std::shared_ptr<Mesh> Primitives::generatePyramid(const uint32_t resolution, const glm::vec2 uvScale) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    const float height = 1.0f;
    const float halfBase = 0.5f;
    const glm::vec3 apex(0.0f, height, 0.0f);
    // Generate vertices for the base
    const float sideLength = 1.0f / resolution;
    for (uint32_t y = 0; y <= resolution; ++y) {
        for (uint32_t x = 0; x <= resolution; ++x) {
            glm::vec3 position(-halfBase + x * sideLength, 0.0f, -halfBase + y * sideLength);
            glm::vec3 normal(0.0f, -1.0f, 0.0f);
            glm::vec2 uv(x / resolution, y / resolution);
            vertices.emplace_back(Vertex{ position, normal, uv * uvScale });
        }
    }
    // Generate indices for the base
    for (uint32_t y = 0; y < resolution; ++y) {
        for (uint32_t x = 0; x < resolution; ++x) {
            const uint32_t topLeft = y * (resolution + 1) + x;
            const uint32_t topRight = topLeft + 1;
            const uint32_t bottomLeft = (y + 1) * (resolution + 1) + x;
            const uint32_t bottomRight = bottomLeft + 1;
            indices.emplace_back(topLeft);
            indices.emplace_back(topRight);
            indices.emplace_back(bottomLeft);
            indices.emplace_back(topRight);
            indices.emplace_back(bottomRight);
            indices.emplace_back(bottomLeft);
        }
    }
    // Generate vertices and indices for the triangular faces
    for (uint32_t i = 0; i < 4; ++i) {
        // Define the corners of the face
        glm::vec3 corner1, corner2;
        glm::vec3 normal;
        switch (i) {
        case 0: // Front face
            corner1 = glm::vec3(-halfBase, 0.0f, halfBase);
            corner2 = glm::vec3(halfBase, 0.0f, halfBase);
            normal = glm::normalize(glm::cross(corner2 - apex, corner1 - apex));
            break;
        case 1: // Right face
            corner1 = glm::vec3(halfBase, 0.0f, halfBase);
            corner2 = glm::vec3(halfBase, 0.0f, -halfBase);
            normal = glm::normalize(glm::cross(corner2 - apex, corner1 - apex));
            break;
        case 2: // Back face
            corner1 = glm::vec3(halfBase, 0.0f, -halfBase);
            corner2 = glm::vec3(-halfBase, 0.0f, -halfBase);
            normal = glm::normalize(glm::cross(corner2 - apex, corner1 - apex));
            break;
        case 3: // Left face
            corner1 = glm::vec3(-halfBase, 0.0f, -halfBase);
            corner2 = glm::vec3(-halfBase, 0.0f, halfBase);
            normal = glm::normalize(glm::cross(corner2 - apex, corner1 - apex));
            break;
        }
        // Add vertices for the face
        uint32_t baseIndex = static_cast<uint32_t>(vertices.size());
        vertices.emplace_back(Vertex { apex, normal, glm::vec2(0.5f, 1.0f) * uvScale });
        vertices.emplace_back(Vertex { corner1, normal, glm::vec2(0.0f, 0.0f) * uvScale });
        vertices.emplace_back(Vertex { corner2, normal, glm::vec2(1.0f, 0.0f) * uvScale });
        // Add indices for the face
        indices.emplace_back(baseIndex);
        indices.emplace_back(baseIndex + 1);
        indices.emplace_back(baseIndex + 2);
    }
    return std::make_shared<Mesh>(vertices, indices, GL_TRIANGLES);
}

std::shared_ptr<Mesh> Primitives::generateSphere(const uint32_t resolution, const glm::vec2 uvScale) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    for (uint32_t lat = 0; lat <= resolution; ++lat) {
        const float theta = glm::pi<float>() * lat / resolution;
        const float sinTheta = std::sin(theta);
        const float cosTheta = std::cos(theta);
        for (uint32_t lon = 0; lon <= resolution; ++lon) {
            const float phi = 2.0f * glm::pi<float>() * lon / resolution;
            const float sinPhi = std::sin(phi);
            const float cosPhi = std::cos(phi);
            const glm::vec3 normal = glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
            const glm::vec3 position = normal;
            const glm::vec2 uv(float(lon) / resolution, float(lat) / resolution);
            vertices.emplace_back(Vertex { position, normal, uv * uvScale });
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
    return std::make_shared<Mesh>(vertices, indices, GL_TRIANGLES);
}

std::shared_ptr<Mesh> Primitives::generateCylinder(const float bottomRadius, const float topRadius, const float length, const uint32_t slices, const uint32_t stacks, const glm::vec2 uvScale) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    // Calculate step sizes for slicing and stacking
    const float sliceStep = glm::pi<float>() * 2.0f / slices;
    const float heightStep = length / stacks;
    const float radiusStep = (topRadius - bottomRadius) / stacks;
    // Start at the bottom of the cylinder
    vertices.emplace_back(Vertex{ glm::vec3(0.0f, -length / 2.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.5f, 0.5f) * uvScale });
    // Vertices for the sides and the top
    float currentHeight = -length / 2.0f;
    float currentRadius = bottomRadius;
    // For each stack layer
    for (uint32_t i = 0; i <= stacks; i++) {
        float sliceAngle = 0.0f;
        for (uint32_t j = 0; j < slices; j++) {
            // Calculate vertex position for current slice
            const float x = currentRadius * std::cos(sliceAngle);
            const float y = currentHeight;
            const float z = currentRadius * std::sin(sliceAngle);
            // Create a vertex and add it to the vertex list
            vertices.emplace_back(Vertex{ glm::vec3(x, y, z), glm::normalize(glm::vec3(x, y, z)), glm::vec2(static_cast<float>(j) / slices, static_cast<float>(i) / stacks) * uvScale });
            sliceAngle += sliceStep;
        }
        // Move to the next height level and radius
        currentHeight += heightStep;
        currentRadius += radiusStep;
    }
    // Top center vertex
    vertices.emplace_back(Vertex{ glm::vec3(0.0f, length / 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.5f) * uvScale });
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
    baseIndex = static_cast<uint32_t>(vertices.size()) - slices - 1;
    for (uint32_t i = 0; i < slices; i++) {
        indices.emplace_back(vertices.size() - 1);
        indices.emplace_back(baseIndex + (i + 1) % slices);
        indices.emplace_back(baseIndex + i);
    }
    return std::make_shared<Mesh>(vertices, indices, GL_TRIANGLES);
}

std::shared_ptr<Mesh> Primitives::generateCone(const float radius, const float length, const int slices, const int stacks, const glm::vec2 uvScale) {
	return Primitives::generateCylinder(radius, 0.0f, length, slices, stacks, uvScale);
}

std::shared_ptr<Mesh> Primitives::generateThorus(const float innerRadius, const float circleRadius, const uint32_t resCircle, const uint32_t resSteps, const glm::vec2 uvScale) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    const float stepCircle = glm::two_pi<float>() / resCircle; // Angle step for the circle
    const float stepSteps = glm::two_pi<float>() / resSteps;   // Angle step for the torus
    for (uint32_t i = 0; i <= resSteps; ++i) {
        const float angleStep = i * stepSteps;
        const glm::vec3 center = glm::vec3(
            innerRadius * std::cos(angleStep),
            0.0f,
            innerRadius * std::sin(angleStep)
        );
        for (uint32_t j = 0; j <= resCircle; ++j) {
            const float angleCircle = j * stepCircle;
            const glm::vec3 circlePoint = glm::vec3(
                std::cos(angleCircle) * circleRadius,
                std::sin(angleCircle) * circleRadius,
                0.0f
            );
            const glm::vec3 position = center + glm::vec3(
                circlePoint.x * std::cos(angleStep) - circlePoint.z * std::sin(angleStep),
                circlePoint.y,
                circlePoint.x * std::sin(angleStep) + circlePoint.z * std::cos(angleStep)
            );
            const glm::vec3 normal = glm::normalize(circlePoint);
            const glm::vec2 texCoord = glm::vec2(static_cast<float>(j) / resCircle, static_cast<float>(i) / resSteps);
            vertices.emplace_back(Vertex{ position, normal, texCoord * uvScale });
        }
    }
    for (uint32_t i = 0; i < resSteps; ++i) {
        for (uint32_t j = 0; j < resCircle; ++j) {
            const uint32_t current = i * (resCircle + 1) + j;
            const uint32_t next = current + resCircle + 1;
            indices.emplace_back(current);
            indices.emplace_back(current + 1);
            indices.emplace_back(next);
            indices.emplace_back(current + 1);
            indices.emplace_back(next + 1);
            indices.emplace_back(next);
        }
    }
	return std::make_shared<Mesh>(vertices, indices, GL_TRIANGLES);
}
