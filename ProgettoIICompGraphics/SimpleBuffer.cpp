#include "SimpleBuffer.hpp"

#include <glad/glad.h>

uint32_t SimpleBuffer::generateBuffer() {
	uint32_t id;
	glGenBuffers(1, &id);
	return id;
}

SimpleBuffer::SimpleBuffer(const uint32_t _type, const bool dynamic)
	:
	id(SimpleBuffer::generateBuffer()),
	type(_type),
	isDynamic(dynamic)
{}

SimpleBuffer::~SimpleBuffer() {
	glDeleteBuffers(1, &this->id);
}

void SimpleBuffer::bind() const {
	glBindBuffer(this->type, this->id);
}

void SimpleBuffer::unbind() const {
	glBindBuffer(this->type, 0);
}