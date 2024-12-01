#include "Mesh3D.hpp"
#include "Vertex.hpp"

#include <glad/glad.h>

Mesh3D::Mesh3D(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const uint32_t _drawType)
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

void Mesh3D::setVertexArrayAttributes() const {
	// Link the vertice's attributes to slots: (0 = vec2 position, 1 = vec2 normal, 2 = vec2 uv)
	this->vao.linkAttrib(0, 3, sizeof(Vertex3D), GL_FLOAT, 0);
	this->vao.linkAttrib(1, 3, sizeof(Vertex3D), GL_FLOAT, 3 * sizeof(float));
	this->vao.linkAttrib(2, 2, sizeof(Vertex3D), GL_FLOAT, 6 * sizeof(float));
}
