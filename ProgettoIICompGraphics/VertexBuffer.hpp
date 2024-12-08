#pragma once

#include "SimpleBuffer.hpp"
#include <vector>

/**
 * Forward declaration for the Vertex2D struct type.
 */
struct Vertex;

class VertexBuffer : public SimpleBuffer {
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
	VertexBuffer(const std::vector<Vertex>& vertices, const bool dynamic = false);
};