#include "TextureCubemap.hpp"

#include <stdexcept>

TextureCubemap::TextureCubemap(const uint32_t bindingPoint)
	:
	Texture(GL_TEXTURE_CUBE_MAP, bindingPoint),
	cubemapTextures{
			Texture2D { 0, GL_RED, GL_RED, GL_TEXTURE_CUBE_MAP_POSITIVE_X },
			Texture2D { 1, GL_RED, GL_RED, GL_TEXTURE_CUBE_MAP_NEGATIVE_X },
			Texture2D { 2, GL_RED, GL_RED, GL_TEXTURE_CUBE_MAP_POSITIVE_Y },
			Texture2D { 3, GL_RED, GL_RED, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y },
			Texture2D { 4, GL_RED, GL_RED, GL_TEXTURE_CUBE_MAP_POSITIVE_Z },
			Texture2D { 5, GL_RED, GL_RED, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z },
		}
{
	// Set base parameters to all textures
	for (const Texture2D& texture : cubemapTextures) {
		texture.bind();
		texture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		texture.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		texture.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		texture.setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		texture.setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

void TextureCubemap::uploadData(const uint32_t width, const uint32_t height, void* data, const uint32_t face) const {
	if (face >= 6) {
		throw std::runtime_error("The face value passed to the cubemap does not exist");
	}
	cubemapTextures[face].uploadData(width, height, data, false);
}
