#pragma once

#include "BoundingBox.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "ElementBuffer.hpp"

class Mesh {
protected:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
public:
	const uint32_t drawType;
	const VertexArray vao;
	const VertexBuffer vbo;
	const ElementBuffer ebo;
	const BoundingBox aabb;
public:
	// Erase copy constructors, as it would break opengl
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	/**
	 * Creates a mesh with the given data.
	 *
	 * \param vertices The vertices that it is composed of.
	 * \param indices The indices to connect those vertices.
	 * \param _drawType The type of OpenGL shape it will draw.
	 */
	Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const uint32_t _drawType);

	/**
	 * Getter for the mesh's bounding box.
	 * 
	 * \return The mesh's bounding box
	 */
	const BoundingBox& getBoundingBox() const;

	/**
	 * Draws the object to the screen.
	 *
	 */
	virtual void draw() const;
private:
	/**
	 * Function to set the VAO vertices' attributes.
	 *
	 */
	virtual void setVertexArrayAttributes() const;
};