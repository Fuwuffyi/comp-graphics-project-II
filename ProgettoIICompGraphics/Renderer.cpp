#include "Renderer.hpp"

#include <glad/glad.h>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "IRenderable.hpp"
#include "RenderingQueue.hpp"

namespace Renderer {
	static RenderingQueue litQueue;
	static RenderingQueue litTransparentQueue(false);
	static RenderingQueue unlitQueue;
	static RenderingQueue unlitTransparentQueue(false);
	static std::vector<IRenderable *> renderables;

	static Material* cubemapMaterial = nullptr;
	static Mesh* cubemapMesh = nullptr;

	static void sendDataToQueues();
}

void Renderer::addToRenderingQueues(IRenderable* renderable) {
	renderables.emplace_back(renderable);
}

void Renderer::sendDataToQueues() {
	for (IRenderable* renderable : renderables) {
		std::vector<std::tuple<Mesh*, Material*, glm::mat4>> drawables = renderable->getDrawables();
		for (auto [meshPtr, materialPtr, model] : drawables) {
			if (materialPtr->getShader()->litFlag) {
				if (materialPtr->getShader()->transparentFlag) {
					litTransparentQueue.addRenderable(meshPtr, materialPtr, model);
				} else {
					litQueue.addRenderable(meshPtr, materialPtr, model);
				}
			} else {
				if (materialPtr->getShader()->transparentFlag) {
					unlitTransparentQueue.addRenderable(meshPtr, materialPtr, model);
				} else {
					unlitQueue.addRenderable(meshPtr, materialPtr, model);
				}
			}
		}
	}
}

void Renderer::setCubemap(Mesh* mesh, Material* material) {
	cubemapMaterial = material;
	cubemapMesh = mesh;
}

void Renderer::setupOpengl() {
	// Set depth testing function
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Cull back faces (!!! ONLY COUNTER CLOCKWISE FACES ARE RENDERED !!!)
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	// Set blending function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::toggleWireframe() {
	static bool wireframe = false;
	wireframe = !wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}

void Renderer::renderAll(const glm::mat4& cameraMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& viewPoint) {
	// Send renderables to queues
	sendDataToQueues();
	// Draw skybox
	if (cubemapMaterial && cubemapMesh) {
		// Disable depth mask for cubemap and culling
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);
		// Draw cubemap using material
		cubemapMaterial->activate();
		cubemapMaterial->getShader()->setUniformMatrix("projectionMatrix", projectionMatrix);
		cubemapMaterial->getShader()->setUniformMatrix("viewMatrix", viewMatrix);
		cubemapMesh->draw();
		// Re-enable other stuff for rendering
		glEnable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
	}
	// Render opaque objects
	litQueue.render(cameraMatrix, viewPoint);
	litQueue.clear();
	unlitQueue.render(cameraMatrix, viewPoint);
	unlitQueue.clear();
	// Enable blending for transparency
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	// Render transparent objects
	litTransparentQueue.render(cameraMatrix, viewPoint);
	litTransparentQueue.clear();
	unlitTransparentQueue.render(cameraMatrix, viewPoint);
	unlitTransparentQueue.clear();
	// Disable blending for transparency
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
