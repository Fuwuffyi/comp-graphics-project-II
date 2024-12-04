#pragma once

#include "SimpleBuffer.hpp"

#include <vector>

/**
 * Forward declaration for the Vertex2D struct type.
 */
struct Vertex2D;

/**
 * Forward declaration for the Vertex3D struct type.
 */
struct Vertex3D;

/**
 * Class to hold the information of a VertexBuffer (the vertices of an object).
 * 
 * \tparam T The vertex type of the buffer, can only be of type Vertex2D and Vertex3D
 */
template <typename T>
class VertexBuffer : public SimpleBuffer {
// Make sure the template is only Vertex2D or Vertex3D
static_assert(
	std::is_same_v<T, Vertex2D> || std::is_same_v<T, Vertex3D>,
	"T must be either Vertex2D or Vertex3D."
);
public:
	// Erase copy constructors, as it would break opengl
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	/**
	 * Constructor for the vertex buffer.
	 *
	 * \param vertices The vertices to save in the GPU buffer.
	 * \param dynamic Flag to check if the data can be overwritten.
	 */
	VertexBuffer(const std::vector<T>& vertices, const bool dynamic = false);
};