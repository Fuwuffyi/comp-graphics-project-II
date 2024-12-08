#include "UniformBuffer.hpp"

#include <glad/glad.h>

UniformBuffer::UniformBuffer(const bool dynamic) 
	:
	SimpleBuffer(GL_UNIFORM_BUFFER, dynamic)
{}

void UniformBuffer::activate(const uint32_t bindingPoint) const {
	glBindBufferBase(this->type, bindingPoint, this->id);
}

void UniformBuffer::uploadData(const size_t size) const {
	this->uploadData(nullptr, size);
}

void UniformBuffer::uploadData(const void* data, const size_t size) const {
	glBufferData(this->type, size, data, this->isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

void UniformBuffer::uploadSubData(const void* data, const size_t dataSize, const size_t offset) const {
	glBufferSubData(this->type, offset, dataSize, data);
}