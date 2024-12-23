#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class Texture {
private:
	static uint32_t genTextureId(const int32_t textureType);
public:
	const uint32_t textureId;
	const int32_t textureType;
public:
	static std::shared_ptr<Texture> dummyTexture;

	// Erase copy constructors, as it would break opengl
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	/**
	 * Creates a texture object.
	 * 
	 * \param _textureType The OpenGL type of the texture to store.
	 */
	Texture(const int32_t _textureType);

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
	 * \param value The parameter's value (e.g.: GL_LINEAR).
	 */
	void setParameter(const int32_t type, const int32_t value) const;

	/**
	 * Sets texture parameters like filtering and wrapping (virtual to allow override).
	 * Make sure the texture is bound first.
	 * 
	 * \param parameters The parameters to set on the texture (pairs of type, value)
	 */
	void setParameters(const std::vector<std::pair<int32_t, int32_t>>& parameters) const;

	/** 
	 * Sets texture to binding point.
	 * 
	 * \param bindingPoint The point to set the texture to for shaders.
	 */
	void activate(const int32_t bindingPoint) const;

	/**
	 * Binds a texture to OpenGL to edit.
	 * 
	 */
	void bind() const;

	/**
	 * Removes texture from binding point.
	 *
	 * \param bindingPoint Removes a texture from a binding
	 */
	void deactivate(const int32_t bindingPoint) const;

	/**
	 * Unbinds the texture
	 *
	 */
	void unbind() const;
};
