#pragma once

#include <glm/glm.hpp>
#include <memory>

/**
 * Foward declaration of the IRenderable interface.
 */
class MeshInstanceNode;

/**
 * Foward declaration of the mesh class.
 */
class Mesh;

/**
 * Foward declaration of the material class.
 */
class Material;

namespace Renderer {
	/**
	 * Toggles between wireframe and normal mode.
	 */
	void toggleWireframe();

	/**
	 * Adds a renderable to the correct rendering queue.
	 * 
	 * \param renderable The renderable to add.
	 */
	void addToRenderingQueues(MeshInstanceNode* renderable);

	/**
	 * Sets up the base opengl draw parameters.
	 */
	void setupOpengl();

	/**
	 * Renders all of the objects present in the renderer.
	 * 
	 * \param cameraMatrix The camera's combined matrix.
	 * \param viewMatrix The camera's view matrix.
	 * \param projectionMatrix The camera's projection matrix.
	 * \param viewPoint The view point in the scene.
	 */
	void renderAll(const glm::mat4& cameraMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& viewPoint);
	
	/**
	 * Getter to return all the currently rendered renderables.
	 * 
	 * \return All the renderer's loaded renderables.
	 */
	const std::vector<MeshInstanceNode*>& getAllRenderables();

	/**
	 * Changes the cubemap of the world.
	 * 
	 * \param mesh The cubemap's mesh.
	 * \param material The cubemap's material
	 */
	void setCubemap(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
};
