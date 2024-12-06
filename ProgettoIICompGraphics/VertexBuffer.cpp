#include "VertexBuffer.hpp"

#include "Vertex.hpp"

#include <glad/glad.h>

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices, const bool dynamic)
	:
	SimpleBuffer(GL_ARRAY_BUFFER, dynamic)
{
	this->bind();
	glBufferData(this->type, static_cast<int64_t>(vertices.size() * sizeof(Vertex)), vertices.data(), dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}