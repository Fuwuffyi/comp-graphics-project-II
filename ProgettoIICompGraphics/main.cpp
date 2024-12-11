#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "CameraControls.hpp"
#include "GUI.hpp"
#include "LightSystem.hpp"
#include "MainScene.hpp"
#include "MaterialLoader.hpp"
#include "Mesh.hpp"
#include "MeshInstanceNode.hpp"
#include "Primitives.hpp"
#include "Renderer.hpp"
#include "SceneNode.hpp"
#include "ShaderLoader.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"
#include "Transform.hpp"
#include "Vertex.hpp"
#include "Window.hpp"
#include <deque>
#include <iostream>

int main() {
	// Initialize glfw
	if (!glfwInit()) {
		std::cerr << "Could not initialize glfw!" << std::endl;
		return EXIT_FAILURE;
	}
	// Set opengl version to V3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Add double buffering
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	// Create the window
	const std::string windowName = "Opengl 3D project";
	Window window(windowName, glm::uvec2(900, 900));
	window.setWindowActive();
	// Create a camera
	Camera cam(Transform(glm::vec3(0.0f, 10.0f, -10.0f), glm::vec3(0.0f, 180.0f, 0.0f)), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 1.0f, 0.01f, 100.0f);
	// Setup GUI
	GUI gui(window.getWindowPtr());
	// Create an empty scene
	std::shared_ptr<SceneNode> scene = MainScene::getScene();
	// Add all the renderables to the renderer
	std::deque<std::shared_ptr<SceneNode>> meshQueue = { scene };
	while (!meshQueue.empty()) {
		auto node = meshQueue.front();
		meshQueue.pop_front();
		if (auto meshInstanceChild = std::dynamic_pointer_cast<MeshInstanceNode>(node)) {
			Renderer::addToRenderingQueues(meshInstanceChild.get());
		}
		for (auto& child : node->getChildren()) {
			meshQueue.push_back(child);
		}
	}
	// Initialize light System
	LightSystem::initialize();
	LightSystem::setLight(0, LightSystem::DirectionalLight{ 
		glm::vec3(-0.52f, -0.62f, -0.5f),
		glm::vec3(0.0f, 0.16f, 0.31f), 
		glm::vec3(0.2f, 0.42f, 0.4f), 
		glm::vec3(0.19f, 0.35f, 0.35f)
	});
	// Add more lights to reflect current scene
	for (uint32_t i = 0; i < 8; ++i) {
		LightSystem::setLight(1 + i, LightSystem::SpotLight{ 
			glm::vec3(6.5f + 3.8f * i, 4.0f, 0.7f), 
			glm::vec3(0.0f, -1.0f, 0.0f),
			0.85f, 0.35f,
			glm::vec3(1.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 0.95f, 0.6f),
			glm::vec3(1.0f, 1.0f, 0.0f),
			15.0f, 0.2f, 0.65f, 0.2f
		});
	}
	for (uint32_t i = 0; i < 8; ++i) {
		LightSystem::setLight(9 + i, LightSystem::SpotLight{
			glm::vec3(-6.5f - 3.8f * i, 4.0f, 0.7f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			0.85f, 0.35f,
			glm::vec3(1.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 0.95f, 0.6f),
			glm::vec3(1.0f, 1.0f, 0.0f),
			15.0f, 0.2f, 0.65f, 0.2f
		});
	}
	LightSystem::setLight(17, LightSystem::PointLight{
		glm::vec3(1.0f, 6.47f, 0.3f),
		glm::vec3(0.05f, 0.0f, 0.0f),
		glm::vec3(0.4f, 0.0f, 0.5f),
		glm::vec3(1.0f, 0.0f, 1.0f),
		15.0f, 0.015f, 0.02f, 0.1f
		});
	// Setup cubemap
	std::shared_ptr<Mesh> cubemapMesh = Primitives::generateCube(1);
	Renderer::setCubemap(cubemapMesh, MaterialLoader::load("cubemap"));
	// Setup dummy texture
	Texture::dummyTexture = TextureLoader::load("dummy.png");
	// Add objects to rendering queue
	Renderer::setupOpengl();
	// Start the draw loop
	double prevTime = glfwGetTime();
	while (!window.shouldClose()) {
		// Deltatime calculation
		const double currTime = glfwGetTime();
		const float deltaTime = static_cast<float>(currTime - prevTime);
		prevTime = currTime;
		// Set widnow title to FPS
		window.setTitle(windowName + " - " + std::to_string(1.0f / deltaTime) + " FPS");
		// Camera movement
		cam.setAspectRatio(static_cast<float>(window.getDimensions().x) / static_cast<float>(window.getDimensions().y));
		if (!gui.clickedOnUi()) {
			CameraControls::cameraControls(cam, window, deltaTime);
		}
		// Clear buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Get new GUI Frame
		gui.newFrame(window.getDimensions());
		// Test draw
		Renderer::renderAll(cam.getCameraMatrix(), cam.getViewMatrix(), cam.getProjectionMatrix(), cam.getTransform().getPosition());
		// Draw gui
		gui.drawLightsEditor();
		gui.drawInspector(scene.get());
		gui.drawResources();
		gui.drawSelection(CameraControls::getSelection());
		gui.endRendering();
		// End frame
		window.swapBuffers();
		glfwPollEvents();
	}
	MaterialLoader::unloadAll();
	ShaderLoader::unloadAll();
	TextureLoader::unloadAll();
	return EXIT_SUCCESS;
}