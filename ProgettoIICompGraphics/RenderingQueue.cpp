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

void RenderingQueue::addRenderable(Mesh* mesh, Material* material, const glm::mat4& modelMatrix) {
	this->renderables.emplace_back(std::make_tuple(mesh, material, modelMatrix));
}

void RenderingQueue::render(const glm::mat4& cameraMatrix, const glm::vec3& viewPoint) {
	// Sort objects for quick rendering
	std::sort(this->renderables.begin(), this->renderables.end(), [this, &viewPoint](const auto& first, const auto& second) {
		const glm::vec3 position1 = glm::vec3(std::get<2>(first)[3]);
		const glm::vec3 position2 = glm::vec3(std::get<2>(second)[3]);
		const glm::vec3 point1 = viewPoint - position1;
		const glm::vec3 point2 = viewPoint - position2;
		return this->closestFirst 
			? glm::dot(point1, point1) < glm::dot(point2, point2)
			: glm::dot(point1, point1) > glm::dot(point2, point2);
	});
	// Render all objects
	for (auto [meshPtr, materialPtr, model] : this->renderables) {
		materialPtr->activate();
		materialPtr->getShader()->setUniform("cameraPosition", viewPoint);
		materialPtr->getShader()->setUniformMatrix("cameraMatrix", cameraMatrix);
		materialPtr->getShader()->setUniformMatrix("objMatrix", model);
		meshPtr->draw();
	}
}

void RenderingQueue::clear() {
	this->renderables.clear();
}

std::size_t RenderingQueue::size() const {
	return this->renderables.size();
}
