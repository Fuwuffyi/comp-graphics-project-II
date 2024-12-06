#include "RenderingQueue.hpp"

#include "IRenderable.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

#include <algorithm>

RenderingQueue::RenderingQueue(const bool _closestFirst)
	:
	closestFirst(_closestFirst),
	renderables()
{}

void RenderingQueue::addRenderable(IRenderable* renderable) {
	this->renderables.emplace_back(renderable);
}

void RenderingQueue::render(const glm::mat4& cameraMatrix, const glm::vec3& viewPoint) {
	std::vector<std::tuple<Mesh*, Material*, glm::mat4>> allRenderingObjects;
	// Get all rendering objects
	for (IRenderable* renderable : this->renderables) {
		auto drawables = renderable->getDrawables();
		allRenderingObjects.insert(allRenderingObjects.end(), drawables.begin(), drawables.end());
	}
	// Sort objects for quick rendering
	std::sort(allRenderingObjects.begin(), allRenderingObjects.end(), [this, &viewPoint](const auto& first, const auto& second) {
		const glm::vec3 position1 = glm::vec3(std::get<2>(first)[3]);
		const glm::vec3 position2 = glm::vec3(std::get<2>(second)[3]);
		return this->closestFirst 
			? glm::distance(position1, viewPoint) < glm::distance(position2, viewPoint) 
			: glm::distance(position1, viewPoint) > glm::distance(position2, viewPoint);
	});
	// Render all objects
	for (auto [meshPtr, materialPtr, model] : allRenderingObjects) {
		materialPtr->activate();
		materialPtr->getShader()->setUniformMatrix("cameraMatrix", cameraMatrix);
		materialPtr->getShader()->setUniformMatrix("objMatrix", model);
		meshPtr->draw();
	}
}
