#pragma once

#include <glm/glm.hpp>
#include <vector>

class IRenderable;

class RenderingQueue {
private:
	std::vector<IRenderable *> renderables;

	const bool closestFirst;
public:
	RenderingQueue(const bool _closestFirst = true);

	void addRenderable(IRenderable* renderable);
	void render(const glm::mat4& cameraMatrix, const glm::vec3& viewPoint);
};
