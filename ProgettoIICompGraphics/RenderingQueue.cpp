#include "RenderingQueue.hpp"

#include "LightSystem.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include <algorithm>
#include <glad/glad.h>
#include <glfw/glfw3.h>

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
			? glm::distance(viewPoint, point1) > glm::distance(viewPoint, point2)
			: glm::distance(viewPoint, point1) < glm::distance(viewPoint, point2);
	});
	// Render all objects
	for (auto [meshPtr, materialPtr, model] : this->renderables) {
		materialPtr->activate();
		// Activate lighting
		LightSystem::enableAt(0);
		const uint32_t blockIndex = glGetUniformBlockIndex(materialPtr->getShader()->id, "lightBuffer");
		glUniformBlockBinding(materialPtr->getShader()->id, blockIndex, 0);
		// Continue rendering normally
		materialPtr->getShader()->setUniform("glfwTime", static_cast<float>(glfwGetTime()));
		materialPtr->getShader()->setUniform("cameraPosition", viewPoint);
		materialPtr->getShader()->setUniform("cameraMatrix", cameraMatrix);
		materialPtr->getShader()->setUniform("objMatrix", model);
		meshPtr->draw();
		materialPtr->deactivate();
	}
}

void RenderingQueue::clear() {
	this->renderables.clear();
}