#pragma once

#include <glm/glm.hpp>
#include <array>

/**
 * Forward declaration for the vertex struct.
 */
struct Vertex;

class BoundingBox {
private:
	/*
	* Contains the max/min values of the coordinates in this order:
	* minX
	* maxX
	* minY
	* maxY
	* minZ
	* maxZ.
	*/
	std::array<float, 6> maxMinValues;
public:
	/**
	 * Constructor for an axis aligned bounding box.
	 *
	 * \param vertices The vertices to calculate the bounding box size from.
	 */
	BoundingBox(const std::vector<Vertex>& vertices);

	/**
	 * Creates a new bounding box given a transformation matrix.
	 *
	 * \param transformationMatrix The transofrmation matrix to create the bounding box from.
	 * \return A new bounding box transformed based on the matrix.
	 */
	BoundingBox transform(const glm::mat4& transformationMatrix) const;

	/**
	 * Checks collision between this and another bounding box.
	 *
	 * \param other The other bounding box to check collisions with.
	 * \return True if the two bounding boxes are colliding.
	 */
	bool checkCollisions(const BoundingBox& other) const;

	/**
	 * Checks collision between itself and a point.
	 *
	 * \param point The point to check the collision with.
	 * \return True they are colliding.
	 */
	bool checkCollisions(const glm::vec3& point) const;
};
