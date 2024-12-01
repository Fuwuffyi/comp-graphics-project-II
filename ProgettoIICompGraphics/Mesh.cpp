#include "Mesh.hpp"

#include "Vertex.hpp"

#include <glad/glad.h>

template <typename T>
Mesh<T>::Mesh(const std::vector<T>& vertices, const std::vector<uint32_t>& indices, const uint32_t _drawType)
	:
	vertices(vertices),
	indices(indices),
	drawType(_drawType),
	vao(),
	vbo(this->vertices),
	ebo(this->indices)
{
	this->vao.bind();
	this->vbo.bind();
	this->ebo.bind();
	this->setVertexArrayAttributes();
	this->vao.unbind();
	this->vbo.unbind();
	this->ebo.unbind();
}

template <typename T>
void Mesh<T>::draw() const {
	this->vao.bind();
	glDrawElements(this->drawType, static_cast<int32_t>(indices.size()), GL_UNSIGNED_INT, 0);
}

template <>
void Mesh2D::setVertexArrayAttributes() const {
	// Link the vertice's attributes to slots: (0 = vec2 position, 1 = vec2 normal, 2 = vec2 uv)
	this->vao.linkAttrib(0, 2, sizeof(Vertex2D), GL_FLOAT, 0);
	this->vao.linkAttrib(1, 2, sizeof(Vertex2D), GL_FLOAT, 2 * sizeof(float));
	this->vao.linkAttrib(2, 2, sizeof(Vertex2D), GL_FLOAT, 4 * sizeof(float));
}

template <>
void Mesh3D::setVertexArrayAttributes() const {
	// Link the vertice's attributes to slots: (0 = vec2 position, 1 = vec2 normal, 2 = vec2 uv)
	this->vao.linkAttrib(0, 3, sizeof(Vertex3D), GL_FLOAT, 0);
	this->vao.linkAttrib(1, 3, sizeof(Vertex3D), GL_FLOAT, 3 * sizeof(float));
	this->vao.linkAttrib(2, 2, sizeof(Vertex3D), GL_FLOAT, 6 * sizeof(float));
}

template class Mesh<Vertex2D>;
template class Mesh<Vertex3D>;