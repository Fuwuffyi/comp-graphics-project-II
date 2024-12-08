#pragma once

#include <glm/glm.hpp>
#include <vector>

/**
 * Forward declaration of the bounding box class.
 */
class BoundingBox;

/**
 * Forward declaration of the material class.
 */
class Material;

/**
 * Forward declaration of the mesh class.
 */
class Mesh;

class IRenderable {
public:
	/**
	 * Gets all the objects that need to be rendered to the screen.
	 * 
	 * \return The objects containing: a mesh pointer, a material pointer, the model matrix, the bounding box.
	 */
	virtual std::vector<std::tuple<Mesh *, Material *, glm::mat4, BoundingBox>> getDrawables() = 0;
};