#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "ElementBuffer.hpp"

/**
 * Contains data on how to draw properly a mesh's data.
 * \tparam T The vertex type of the mesh.
 */
template <typename T>
class Mesh {
protected:
	std::vector<T> vertices;
	std::vector<uint32_t> indices;
public:
	const uint32_t drawType;
	const VertexArray vao;
	const VertexBuffer<T> vbo;
	const ElementBuffer ebo;
protected:

	/**
	 * Creates a mesh with the given data.
	 *
	 * \param vertices The vertices that it is composed of.
	 * \param indices The indices to connect those vertices.
	 * \param _drawType The type of OpenGL shape it will draw.
	 */
	Mesh(const std::vector<T>& vertices, const std::vector<uint32_t>& indices, const uint32_t _drawType);
public:

	/**
	 * Draws the object to the screen.
	 *
	 */
	virtual void draw() const;
protected:

	/**
	 * Function to set the VAO vertices' attributes.
	 *
	 */
	virtual void setVertexArrayAttributes() const;
};