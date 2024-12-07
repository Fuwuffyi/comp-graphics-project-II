#include "Texture2D.hpp"

Texture2D::Texture2D(const int32_t _internalFormat, const int32_t _externalFormat, const int32_t cubemapType)
	:
	Texture(cubemapType),
	internalFormat(_internalFormat),
	externalFormat(_externalFormat)
{}

void Texture2D::uploadData(const int32_t width, const int32_t height, const uint8_t* data, const bool genMipMaps) const {
	glTexImage2D(this->textureType, 0, this->internalFormat, width, height, 0, this->externalFormat, GL_UNSIGNED_BYTE, reinterpret_cast<const void *>(data));
	if (genMipMaps) {
		glGenerateMipmap(this->textureType);
	}
}