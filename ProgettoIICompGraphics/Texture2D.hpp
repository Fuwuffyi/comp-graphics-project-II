#pragma once

#include "Texture.hpp"

#include <string>

class Texture2D : public Texture {
public:
	const int32_t internalFormat;
	const int32_t externalFormat;
public:
	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	/**
	 * Creates a Texture2D object with the data of a texture.
	 * 
	 * \param _internalFormat The internal format of the pixels (e.g.: GL_RED).
	 * \param _externalFormat The external format of the pixels (e.g.: GL_RED).
	 * \param cubemapType Overrides the texture type so it can be used for cubemaps.
	 */
	Texture2D(const int32_t _internalFormat, const int32_t _externalFormat, const int32_t cubemapType = GL_TEXTURE_2D);
	
	/**
	 * Uploads texture data from the GPU.
	 * 
	 * \param width The witdth of the data image to put on the gpu.
	 * \param height The height of the data image to put on the gpu.
	 * \param data The data's reference pointer.
	 * \param genMipMaps Flag to generate mipmaps.
	 */
	void uploadData(const int32_t width, const int32_t height, const uint8_t* data, const bool genMipMaps = true) const;
};
