#include "Mesh2D.hpp"
#include "Vertex.hpp"

#include <glad/glad.h>

Mesh2D::Mesh2D(const std::vector<Vertex2D>& vertices, const std::vector<uint32_t>& indices, const uint32_t _drawType) 
	:
	Mesh(vertices, indices, _drawType)
{
	this->vao.bind();
	this->vbo.bind();
	this->ebo.bind();
	this->setVertexArrayAttributes();
	this->vao.unbind();
	this->vbo.unbind();
	this->ebo.unbind();
}

void Mesh2D::setVertexArrayAttributes() const {
	// Link the vertice's attributes to slots: (0 = vec2 position, 1 = vec2 normal, 2 = vec2 uv)
	this->vao.linkAttrib(0, 2, sizeof(Vertex2D), GL_FLOAT, 0);
	this->vao.linkAttrib(1, 2, sizeof(Vertex2D), GL_FLOAT, 2 * sizeof(float));
	this->vao.linkAttrib(2, 2, sizeof(Vertex2D), GL_FLOAT, 4 * sizeof(float));
}