#include "Mesh.hpp"

#include "Vertex.hpp"

#include <glad/glad.h>

#include <iostream>

template <typename T>
Mesh<T>::Mesh(const std::vector<T>& vertices, const std::vector<uint32_t>& indices, const uint32_t _drawType)
	:
	vertices(vertices),
	indices(indices),
	drawType(_drawType),
	vao(),
	vbo(this->vertices),
	ebo(this->indices)
{}

template <typename T>
void Mesh<T>::draw() const {
	this->vao.bind();
	glDrawElements(this->drawType, static_cast<int32_t>(indices.size()), GL_UNSIGNED_INT, 0);
}

template class Mesh<Vertex2D>;
template class Mesh<Vertex3D>;
