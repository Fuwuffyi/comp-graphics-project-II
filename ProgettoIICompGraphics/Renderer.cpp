#include "Renderer.hpp"

#include "MeshInstanceNode.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "RenderingQueue.hpp"
#include "Shader.hpp"
#include <glad/glad.h>

namespace Renderer {
	static RenderingQueue litQueue;
	static RenderingQueue litTransparentQueue(false);
	static RenderingQueue unlitQueue;
	static RenderingQueue unlitTransparentQueue(false);
	static std::vector<MeshInstanceNode *> renderingList;

	static std::shared_ptr<Material> cubemapMaterial = nullptr;
	static Mesh* cubemapMesh = nullptr;

	static void sendDataToQueues();
}

void Renderer::addToRenderingQueues(MeshInstanceNode* renderable) {
	renderingList.emplace_back(renderable);
}

void Renderer::sendDataToQueues() {
	for (MeshInstanceNode* renderable : renderingList) {
		Material* materialPtr = renderable->getMaterial().get();
		const Shader* shaderPtr = renderable->getMaterial()->getShader();
		if (shaderPtr->litFlag) {
			if (shaderPtr->transparentFlag) {
				litTransparentQueue.addRenderable(renderable->getMesh(), materialPtr, renderable->getWorldTransform().getTransformMatrix());
			} else {
				litQueue.addRenderable(renderable->getMesh(), materialPtr, renderable->getWorldTransform().getTransformMatrix());
			}
		} else {
			if (shaderPtr->transparentFlag) {
				unlitTransparentQueue.addRenderable(renderable->getMesh(), materialPtr, renderable->getWorldTransform().getTransformMatrix());
			} else {
				unlitQueue.addRenderable(renderable->getMesh(), materialPtr, renderable->getWorldTransform().getTransformMatrix());
			}
		}
	}
}

void Renderer::setCubemap(Mesh* mesh, const std::shared_ptr<Material>& material) {
	if (mesh) {
		cubemapMesh = mesh;
	}
	if (material) {
		cubemapMaterial = material;
	}
}

const std::vector<MeshInstanceNode *>& Renderer::getAllRenderables() {
	return renderingList;
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
