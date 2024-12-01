#pragma once

#include "Mesh.hpp"

/**
 * Contains data on how to draw properly a mesh's data.
 */
class Mesh3D : public Mesh<Vertex3D> {
public:
	/**
	 * Creates a mesh with the given data.
	 *
	 * \param vertices The vertices that it is composed of.
	 * \param indices The indices to connect those vertices.
	 * \param _drawType The type of OpenGL shape it will draw.
	 */
	Mesh3D(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const uint32_t _drawType);
private:
	/**
	 * Function to set the VAO vertices' attributes.
	 *
	 */
	void setVertexArrayAttributes() const;
};
