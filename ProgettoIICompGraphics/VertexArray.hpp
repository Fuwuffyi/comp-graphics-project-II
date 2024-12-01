#pragma once

#include <cstdint>

/**
 * Class that handles OpenGL's VertexArrays.
 */
class VertexArray {
private:
	/**
	 * Generates the id of the VertexArray.
	 *
	 * \return  The id of the VertexArray;
	 */
	static uint32_t generateBuffer();
public:
	const uint32_t id;

	/**
	 * Creates a new VertexArray.
	 *
	 */
	VertexArray();

	/**
	 * Deallocates the GPU memory that defines this VertexArray.
	 *
	 */
	~VertexArray();

	/**
	 * Links an attribute from a VertexBuffer's data.
	 *
	 * \param layout The layout id of the defined attribute.
	 * \param numComponents The amount of the components the attribute has (1-4)
	 * \param sturctSize The size of the struct passed to this VAO.
	 * \param valueType The value's type (float, int etc..)
	 * \param offset The offset to read from compared to the 0th element of the vertex.
	 */
	void linkAttrib(const uint32_t layout, const uint32_t numComponents, const size_t sturctSize, const uint32_t valueType, const uint32_t offset) const;

	/**
	 * Activates this VertexArray to draw the object.
	 *
	 */
	void bind() const;

	/**
	 * Deactivates this VertexArray to draw the object.
	 *
	 */
	void unbind() const;
};