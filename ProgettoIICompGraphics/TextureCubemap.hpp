#pragma once

#include <array>

#include "Texture2D.hpp"

class TextureCubemap : public Texture {
private:
	// The various cubemap texture faces
	std::array<Texture2D, 6> cubemapTextures;
public:
	TextureCubemap(const TextureCubemap&) = delete;
	TextureCubemap& operator=(const TextureCubemap&) = delete;

	/**
	 * Creates an empty cubemap.
	 *
	 * \param _bindingPoint The binding point of the texture for shaders.
	 */
	TextureCubemap(const int32_t _bindingPoint);

	/**
	 * Uploads texture data from the GPU.
	 *
	 * \param width The witdth of the data image to put on the gpu.
	 * \param height The height of the data image to put on the gpu.
	 * \param data The data's reference pointer.
	 * \param face The face to set the data to.
	 */
	void uploadData(const uint32_t width, const uint32_t height, void* data, const uint32_t face) const;
};