#include "VertexArray.hpp"

#include <glad/glad.h>

uint32_t VertexArray::generateBuffer() {
	uint32_t id;
	glGenVertexArrays(1, &id);
	return id;
}

VertexArray::VertexArray()
	:
	id(VertexArray::generateBuffer())
{}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &this->id);
}

void VertexArray::linkAttrib(const uint32_t layout, const uint32_t numComponents, const size_t sturctSize, const uint32_t valueType, const size_t offset) const {
	glVertexAttribPointer(layout, static_cast<int32_t>(numComponents), valueType, GL_FALSE, static_cast<uint32_t>(sturctSize), reinterpret_cast<void*>(offset));
	glEnableVertexAttribArray(layout);
}

void VertexArray::bind() const {
	glBindVertexArray(this->id);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}