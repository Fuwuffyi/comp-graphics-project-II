#include "Renderer.hpp"

#include "Material.hpp"
#include "IRenderable.hpp"
#include "RenderingQueue.hpp"

namespace Renderer {
	static RenderingQueue litQueue;
	static RenderingQueue litTransparentQueue(false);
	static RenderingQueue unlitQueue;
	static RenderingQueue unlitTransparentQueue(false);
	static std::vector<IRenderable *> renderables;

	static void sendDataToQueues();
}

void Renderer::addToRenderingQueues(IRenderable* renderable) {
	renderables.emplace_back(renderable);
}

void Renderer::sendDataToQueues() {
	for (IRenderable* renderable : renderables) {
		std::vector<std::tuple<Mesh*, Material*, glm::mat4>> drawables = renderable->getDrawables();
		for (auto [meshPtr, materialPtr, model] : drawables) {
			if (materialPtr->isLit()) {
				if (materialPtr->isTransparent()) {
					litTransparentQueue.addRenderable(meshPtr, materialPtr, model);
				}
				litQueue.addRenderable(meshPtr, materialPtr, model);
			}
			else {
				if (materialPtr->isTransparent()) {
					unlitTransparentQueue.addRenderable(meshPtr, materialPtr, model);
				}
				unlitQueue.addRenderable(meshPtr, materialPtr, model);
			}
		}
	}
}

void Renderer::renderAll(const glm::mat4& cameraMatrix, const glm::vec3& viewPoint) {
	sendDataToQueues();
	litQueue.render(cameraMatrix, viewPoint);
	litQueue.clear();
	unlitQueue.render(cameraMatrix, viewPoint);
	unlitQueue.clear();
	litTransparentQueue.render(cameraMatrix, viewPoint);
	litTransparentQueue.clear();
	unlitTransparentQueue.render(cameraMatrix, viewPoint);
	unlitTransparentQueue.clear();
}
