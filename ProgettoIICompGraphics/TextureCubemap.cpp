#include "TextureCubemap.hpp"

#include <stdexcept>

TextureCubemap::TextureCubemap()
	:
	Texture(GL_TEXTURE_CUBE_MAP),
	cubemapTextures{
			Texture2D { GL_RGB, GL_RGB, GL_TEXTURE_CUBE_MAP_POSITIVE_X },
			Texture2D { GL_RGB, GL_RGB, GL_TEXTURE_CUBE_MAP_NEGATIVE_X },
			Texture2D { GL_RGB, GL_RGB, GL_TEXTURE_CUBE_MAP_POSITIVE_Y },
			Texture2D { GL_RGB, GL_RGB, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y },
			Texture2D { GL_RGB, GL_RGB, GL_TEXTURE_CUBE_MAP_POSITIVE_Z },
			Texture2D { GL_RGB, GL_RGB, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z },
		}
{
	this->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	this->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	this->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	this->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	this->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TextureCubemap::uploadData(const int32_t width, const int32_t height, const uint8_t* data, const uint32_t face) const {
	if (face >= 6) {
		throw std::runtime_error("The face value passed to the cubemap does not exist");
	}
	cubemapTextures[face].uploadData(width, height, data, false);
}
