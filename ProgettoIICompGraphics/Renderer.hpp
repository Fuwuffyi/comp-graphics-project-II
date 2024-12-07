#pragma once

#include <glm/glm.hpp>

class IRenderable;
class Mesh;
class Material;

namespace Renderer {
	void toggleWireframe();
	void addToRenderingQueues(IRenderable* renderable);
	void setupOpengl();
	void renderAll(const glm::mat4& cameraMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& viewPoint);

	void setCubemap(Mesh* mesh, Material* material);
};
