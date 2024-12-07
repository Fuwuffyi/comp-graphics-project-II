#pragma once

#include <cstdint>
#include <vector>
#include <glad/glad.h>

class Texture {
private:
	static uint32_t genTextureId(const int32_t textureType);
public:
	const uint32_t textureId;
	const int32_t textureType;
	const uint32_t bindingPoint;
public:
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	/**
	 * Creates a texture object.
	 * 
	 * \param _textureType The OpenGL type of the texture to store.
	 * \param _bindingPoint The texture uniform the texture binds to.
	 */
	Texture(const int32_t _textureType, const uint32_t _bindingPoint);

	/**
	 * Deletes the texture's GPU allocated memory.
	 * 
	 */
	virtual ~Texture();

	/**
	 * Sets texture parameters like filtering and wrapping (virtual to allow override).
	 * Make sure the texture is bound first.
	 * 
	 * \param type The parameter's type (e.g.: GL_TEXTURE_MIN_FILTER).
	 * \param type The parameter's value (e.g.: GL_LINEAR).
	 */
	virtual void setParameter(const int32_t type, const int32_t value) const;

	/**
	 * Sets texture parameters like filtering and wrapping (virtual to allow override).
	 * Make sure the texture is bound first.
	 * 
	 * \param parameters The parameters to set on the texture (pairs of type, value)
	 */
	virtual void setParameters(const std::vector<std::pair<int32_t, int32_t>>& parameters) const;

	/**
	* Binds the texture for use.
	* 
	*/
	virtual void bind() const;

	/**
	* Unbinds the texture.
	* 
	*/
	virtual void unbind() const;
};
