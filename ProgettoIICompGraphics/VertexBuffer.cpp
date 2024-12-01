#include "VertexBuffer.hpp"

#include "Vertex.hpp"

#include <glad/glad.h>

template <typename T>
VertexBuffer<T>::VertexBuffer(const std::vector<T>& vertices, const bool dynamic)
	:
	SimpleBuffer(GL_ARRAY_BUFFER, dynamic)
{
	this->bind();
	glBufferData(this->type, static_cast<int64_t>(vertices.size() * sizeof(T)), vertices.data(), dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

template class VertexBuffer<Vertex2D>;
template class VertexBuffer<Vertex3D>;