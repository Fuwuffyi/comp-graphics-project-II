#include "Texture.hpp"

#include <glad/glad.h>
#include <stdexcept>

uint32_t Texture::genTextureId(const int32_t textureType) {
	uint32_t texture;
	glGenTextures(1, &texture);
	return texture;
}

Texture::Texture(const int32_t _textureType)
	:
	textureId(Texture::genTextureId(_textureType)),
	textureType(_textureType)
{
	this->bind();
}

Texture::~Texture() {
	glDeleteTextures(1, &this->textureId);
}

void Texture::setParameter(const int32_t type, const int32_t value) const {
	glTexParameteri(this->textureType, type, value);
}

void Texture::setParameters(const std::vector<std::pair<int32_t, int32_t>>& parameters) const {
	for (const auto& [type, value] : parameters) {
		this->setParameter(type, value);
	}
}

void Texture::activate(const int32_t bindingPoint) const {
	glActiveTexture(GL_TEXTURE0 + bindingPoint);
	this->bind();
}

void Texture::bind() const {
	glBindTexture(this->textureType, this->textureId);
}

void Texture::deactivate(const int32_t bindingPoint) const {
	glActiveTexture(GL_TEXTURE0 + bindingPoint);
	this->unbind();
}

void Texture::unbind() const {
	glBindTexture(this->textureType, 0);
}
