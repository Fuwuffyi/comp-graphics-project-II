#pragma once

#include <glm/glm.hpp>
#include <memory>

class IRenderable;
class Mesh;
class Material;

namespace Renderer {
	void toggleWireframe();
	void addToRenderingQueues(IRenderable* renderable);
	void setupOpengl();
	void renderAll(const glm::mat4& cameraMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& viewPoint);
	
	const std::vector<IRenderable *>& getAllRenderables();

	void setCubemap(Mesh* mesh, const std::shared_ptr<Material>& material);
};
