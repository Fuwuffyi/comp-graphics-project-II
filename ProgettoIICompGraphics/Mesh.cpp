#include "Mesh.hpp"

#include "Vertex.hpp"

#include <glad/glad.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const uint32_t _drawType)
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

void Mesh::draw() const {
	this->vao.bind();
	glDrawElements(this->drawType, static_cast<int32_t>(indices.size()), GL_UNSIGNED_INT, 0);
}

void Mesh::setVertexArrayAttributes() const {
	// Link the vertices' attributes to slots: (0 = vec2 position, 1 = vec2 normal, 2 = vec2 uv)
	this->vao.linkAttrib(0, 3, sizeof(Vertex), GL_FLOAT, 0);
	this->vao.linkAttrib(1, 3, sizeof(Vertex), GL_FLOAT, 3 * sizeof(float));
	this->vao.linkAttrib(2, 2, sizeof(Vertex), GL_FLOAT, 6 * sizeof(float));
}
