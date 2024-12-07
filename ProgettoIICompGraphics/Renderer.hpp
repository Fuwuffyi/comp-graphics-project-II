#pragma once

#include <glm/glm.hpp>

class IRenderable;

namespace Renderer {
	void addToRenderingQueues(IRenderable* renderable);
	void setupOpengl();
	void renderAll(const glm::mat4& cameraMatrix, const glm::vec3& viewPoint);
};
