#pragma once

#include <glm/glm.hpp>
#include <memory>

/**
 * Forward declaration of the mesh class.
 */
class Mesh;

namespace Primitives {
	/**
	 * Generates a heap allocated plane.
	 * 
	 * \param resolution The amount of subdivisions of the plane.
	 * \return The plane object.
	 */
	std::shared_ptr<Mesh> generatePlane(const uint32_t resolution = 1);

	/**
	 * Generates a heap allocated cube.
	 *
	 * \param resolution The amount of subdivisions of the faces of the cube.
	 * \return The cube object.
	 */
	std::shared_ptr<Mesh> generateCube(const uint32_t resolution = 1);

	/**
	 * Generates a heap allocated pyramid.
	 *
	 * \param resolution Amount of subdivisions on the base of the pyramid.
	 * \return The pyramid object.
	 */
	std::shared_ptr<Mesh> generatePyramid(const uint32_t resolution);

	/**
	 * Generates a heap allocated sphere.
	 *
	 * \param resolution The amount of steps taken on each axis to generate a more detailed sphere.
	 * \return The sphere object.
	 */
	std::shared_ptr<Mesh> generateSphere(const uint32_t resolution = 3);

	/**
	 * Generates a heap allocated cylinder.
	 *
	 * \param bottomRadius The radius of the bottom base of the cylinder.
	 * \param topRadius The radius of the top base of the cylinder.
	 * \param length The length of the cylinder.
	 * \param slices The amount of horizontal detail of the model.
	 * \param stacks The amount of vertical detail of the model.
	 * \return The cylinder object.
	 */
	std::shared_ptr<Mesh> generateCylinder(const float bottomRadius = 1.0f, const float topRadius = 1.0f, const float length = 1.0f, const uint32_t slices = 5, const uint32_t stacks = 5);
	
	/**
	 * Generates a heap allocated cone.
	 *
	 * \param radius The radius of the base of the cone.
	 * \param length The length of the cone.
	 * \param slices The amount of horizontal detail of the cone.
	 * \param stacks The amount of vertical detail of the cone.
	 * \return The cone object.
	 */
	std::shared_ptr<Mesh> generateCone(const float radius = 1.0f, const float length = 1.0f, const int slices = 5, const int stacks = 5);

	/**
	 * Generates a heap allocated thorus.
	 *
	 * \param innerRadius The radius between the center and the thorus shape.
	 * \param circleRadius The radius of the circle of the thorus shape.
	 * \param resCircle Amount of subdivisions of the circle.
	 * \param resSteps Amount of circles to create the thorus.
	 * \return The thorus object.
	 */
	std::shared_ptr<Mesh> generateThorus(const float innerRadius = 1.0f, const float circleRadius = 0.25f, const uint32_t resCircle = 5, const uint32_t resSteps = 5);
}
