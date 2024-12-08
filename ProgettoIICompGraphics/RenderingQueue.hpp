#pragma once

#include <glm/glm.hpp>
#include <vector>

/**
 * Foward declaration of the mesh class.
 */
class Mesh;

/**
 * Foward declaration of the material class.
 */
class Material;

class RenderingQueue {
private:
	std::vector<std::tuple<Mesh*, Material *, glm::mat4>> renderables;

	const bool closestFirst;
public:
	/**
	 * Creates a rendering queue.
	 * 
	 * \param _closestFirst Flag to check if it should draw the closest object first or last.
	 */
	RenderingQueue(const bool _closestFirst = true);

	/**
	 * Adds a renderable to the queue.
	 * 
	 * \param mesh The mesh to draw.
	 * \param material The material to draw the mesh with.
	 * \param modelMatrix The model matrix of the object to render.
	 */
	void addRenderable(Mesh* mesh, Material* material, const glm::mat4& modelMatrix);

	/**
	 * Renders all of the objects in the queue.
	 * 
	 * \param cameraMatrix The camera's combined matrix.
	 * \param viewPoint The point the scene is rendered from.
	 */
	void render(const glm::mat4& cameraMatrix, const glm::vec3& viewPoint);

	/**
	 * Removes all the objects from the queue.
	 * 
	 */
	void clear();
};
