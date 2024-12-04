#pragma once

#include <cstdint>

/**
 * Base class holding a simple OpenGL Buffer.
 */
class SimpleBuffer {
private:
	/**
	 * Generates an OpenGL buffer of the given type.
	 *
	 * \return The id of the generated buffer.
	 */
	static uint32_t generateBuffer();
public:
	const uint32_t id;
	const uint32_t type;
	const bool isDynamic;

	// Erase copy constructors, as it would break opengl
	SimpleBuffer(const SimpleBuffer&) = delete;
	SimpleBuffer& operator=(const SimpleBuffer&) = delete;

	/**
	 * Constructore for the base class.
	 *
	 * \param _type The type of buffer it rapresents.
	 * \param dynamic Flag to check if the buffer's data will be overwritten.
	 */
	SimpleBuffer(const uint32_t _type, const bool dynamic = false);

	/**
	 * Deletes the GPU allocated memory of the buffer.
	 *
	 */
	virtual ~SimpleBuffer();

	/**
	 * Enables the current buffer for use.
	 *
	 */
	virtual void bind() const;

	/**
	 * Disables the current buffer for use.
	 *
	 */
	virtual void unbind() const;
};