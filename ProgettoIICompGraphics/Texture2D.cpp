#include "Texture2D.hpp"

Texture2D::Texture2D(const uint32_t _bindingPoint, const int32_t _internalFormat, const int32_t _externalFormat, const int32_t cubemapType)
	:
	Texture(cubemapType, _bindingPoint),
	bindingPoint(_bindingPoint),
	internalFormat(_internalFormat),
	externalFormat(_externalFormat)
{}

void Texture2D::uploadData(const uint32_t width, const uint32_t height, void* data, const bool genMipMaps) const {
	glTexImage2D(this->textureType, 0, static_cast<int32_t>(this->internalFormat), static_cast<int32_t>(width), static_cast<int32_t>(height), 0, this->externalFormat, GL_UNSIGNED_BYTE, data);
	if (genMipMaps) {
		glGenerateMipmap(this->textureType);
	}
}