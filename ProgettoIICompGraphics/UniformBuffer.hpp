#pragma once

#include "SimpleBuffer.hpp"

class UniformBuffer : public SimpleBuffer {
public:
	// Erase copy constructors, as it would break opengl
	UniformBuffer(const UniformBuffer&) = delete;
	UniformBuffer& operator=(const UniformBuffer&) = delete;

	/**
	 * Creates an uniform buffer object.
	 * 
	 * \param dynamic Flag to check if the buffer changes often.
	 */
	UniformBuffer(const bool dynamic = true);

	/**
	 * Binds the buffer to a binding point for shader use.
	 * 
	 * \param bindingPoint The binding point on the shader.
	 */
	void activate(const uint32_t bindingPoint) const;

	/**
	 * Uploads data to the buffer.
	 * 
	 * \param data The data to pass to the object.
	 * \param size The amount of data to pass to the object.
	 */
	void uploadData(const void* data, const size_t size) const;

	/**
	 * Uploads empty data to the buffer.
	 *
	 * \param size The amount of data to pass to the object.
	 */
	void uploadData(const size_t size) const;

	/**
	 * Uploads data on a specific part of the buffer.
	 * 
	 * \param data The data to pass to the object.
	 * \param dataSize The size of the data that has been passed.
	 * \param offset The offset from the start of the buffer to overwrite the data from.
	 */
	void uploadSubData(const void* data, const size_t dataSize, const size_t offset) const;
};