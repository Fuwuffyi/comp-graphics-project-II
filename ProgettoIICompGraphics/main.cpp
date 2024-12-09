#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "GUI.hpp"
#include "Keyboard.hpp"
#include "LightSystem.hpp"
#include "MaterialLoader.hpp"
#include "Mesh.hpp"
#include "MeshInstanceNode.hpp"
#include "MeshLoader.hpp"
#include "Mouse.hpp"
#include "Primitives.hpp"
#include "Renderer.hpp"
#include "SceneNode.hpp"
#include "ShaderLoader.hpp"
#include "TextureLoader.hpp"
#include "Transform.hpp"
#include "Vertex.hpp"
#include "Window.hpp"
#include <deque>
#include <iostream>

static SceneNode* selectedInstance = nullptr;

void cameraControls(Camera& cam, Window& window, const float deltaTime) {
	const Transform currentTransform = cam.getTransform();
	// Setup movement
	if (Keyboard::key(GLFW_KEY_W)) {
		// Move forward
		cam.getMutableTransform().setPosition(cam.getTransform().getPosition() + cam.getViewDirection() * 2.0f * deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_S)) {
		// Move back
		cam.getMutableTransform().setPosition(cam.getTransform().getPosition() + cam.getViewDirection() * -2.0f * deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_SPACE)) {
		// Move up
		cam.getMutableTransform().setPosition(cam.getTransform().getPosition() + cam.getUpVector() * 2.0f * deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
		// Move down
		cam.getMutableTransform().setPosition(cam.getTransform().getPosition() + cam.getUpVector() * -2.0f * deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_A)) {
		// Move left
		cam.getMutableTransform().setPosition(cam.getTransform().getPosition() + cam.getRightVector() * -2.0f * deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_D)) {
		// Move right
		cam.getMutableTransform().setPosition(cam.getTransform().getPosition() + cam.getRightVector() * 2.0f * deltaTime);
	}
	// Toggle wireframe on right alt
	if (Keyboard::keyWentDown(GLFW_KEY_RIGHT_ALT)) {
		Renderer::toggleWireframe();
	}
	// Set cursor hidden when holding left/middle mouse buttons
	constexpr const float sensitivity = 0.1f;
	if (Mouse::buttonWentDown(GLFW_MOUSE_BUTTON_LEFT)) {
		glfwSetInputMode(window.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} else if (Mouse::buttonWentUp(GLFW_MOUSE_BUTTON_LEFT) || Mouse::buttonWentUp(GLFW_MOUSE_BUTTON_MIDDLE)) {
		glfwSetInputMode(window.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	// Setup FPS style camera when holding left click
	constexpr const float epsilon = 0.01f;
	if (Mouse::button(GLFW_MOUSE_BUTTON_LEFT)) {
		const float mouseDeltaX = Mouse::getDx();
		const float mouseDeltaY = Mouse::getDy();
		const glm::vec3 newRotation = cam.getTransform().getRotation() + glm::vec3(mouseDeltaY, -mouseDeltaX, 0.0f) * sensitivity;
		cam.getMutableTransform().setRotation(glm::vec3(glm::clamp(newRotation.x, -90.0f + epsilon, 90.0f - epsilon), newRotation.y, newRotation.z));
	}
	// Change FOV/Zoom based on scroll wheel
	static float trackballZoom = 1.0f;
	if (Mouse::button(GLFW_MOUSE_BUTTON_MIDDLE)) {
		const float newZoom = trackballZoom - Mouse::getScrollDy() * 100.0f * deltaTime;
		trackballZoom = std::min(std::max(newZoom, epsilon), 10.0f);
	} else {
		const float newFov = cam.getFOV() - Mouse::getScrollDy() * 1000.0f * deltaTime;
		cam.setFOV(std::min(std::max(newFov, epsilon), 120.0f - epsilon));
	}
	// Setup trackball style camera when holding middle mouse button
	static glm::vec3 target = glm::vec3(0.0f);
	if (Mouse::buttonWentDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
		glfwSetInputMode(window.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		target = cam.getTransform().getPosition() + cam.getViewDirection();
	}
	if (Mouse::button(GLFW_MOUSE_BUTTON_MIDDLE)) {
		const float mouseDeltaX = Mouse::getDx();
		const float mouseDeltaY = Mouse::getDy();
		const glm::vec3 currentRotation = cam.getTransform().getRotation();
		const float yaw = currentRotation.y + mouseDeltaX * sensitivity;
		const float pitch = currentRotation.x + mouseDeltaY * sensitivity;
		cam.getMutableTransform().setRotation(glm::vec3(glm::clamp(pitch, -90.0f + epsilon, 89.0f - epsilon), yaw, currentRotation.z));
		const glm::vec3 newPosition = target - cam.getViewDirection() * trackballZoom;
		cam.getMutableTransform().setPosition(newPosition);
	}
	// Check selections
	if (Mouse::buttonWentDown(GLFW_MOUSE_BUTTON_RIGHT)) {
		// Get useful coordinates
		const glm::uvec2 windowDimensions = window.getDimensions();
		const glm::vec2 mousePos = glm::vec2(Mouse::getMouseX(), Mouse::getMouseY());
		glm::vec2 ndcMousePos;
		ndcMousePos.x = (2.0f * mousePos.x) / windowDimensions.x - 1.0f;
		ndcMousePos.y = 1.0f - (2.0f * mousePos.y) / windowDimensions.y;
		// Ray in clip space
		const glm::vec4 rayClip(ndcMousePos.x, ndcMousePos.y, -1.0f, 1.0f);
		// Convert to view space
		const glm::mat4 projMatrix = cam.getProjectionMatrix();
		glm::vec4 rayView = glm::inverse(projMatrix) * rayClip;
		rayView.z = -1.0f; // Ensure ray points forward
		rayView.w = 0.0f;
		// Convert to world space
		const glm::mat4 viewMatrix = cam.getViewMatrix();
		const glm::vec3 rayOrigin = cam.getTransform().getPosition();
		const glm::vec3 rayDirection = glm::normalize(glm::vec3(glm::inverse(viewMatrix) * rayView));
		// Setup some base variables for the search
		MeshInstanceNode* closestInstance = nullptr;
		float closestDistance = std::numeric_limits<float>::max();
		// Loop through all instances and check if the ray intersects any bounding boxes
		const std::vector<MeshInstanceNode*>& instances = Renderer::getAllRenderables();
		for (MeshInstanceNode* instance : instances) {
			float tMin, tMax;
			if (instance->getBoundingBox().rayIntersects(rayOrigin, rayDirection, tMin, tMax)) {
				// Check if this instance is the closest
				if (tMin < closestDistance) {
					closestDistance = tMin;
					closestInstance = instance;
				}
			}
		}
		selectedInstance = selectedInstance == closestInstance ? nullptr : closestInstance;
	}
	// Check collisions
	const std::vector<MeshInstanceNode *>& instances = Renderer::getAllRenderables();
	for (MeshInstanceNode* instance : instances) {
		if (instance->getBoundingBox().checkCollisions(cam.getTransform().getPosition())) {
			cam.getMutableTransform().setPosition(currentTransform.getPosition());
			return;
		}
	}
}

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
	Camera cam(Transform(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 180.0f, 0.0f)), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 1.0f, 0.01f, 100.0f);
	// Setup GUI
	GUI gui(window.getWindowPtr());
	// Create an empty scene
	std::shared_ptr<SceneNode> scene = std::make_shared<SceneNode>("Scene", Transform());
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
	LightSystem::setLight(0, LightSystem::DirectionalLight{ glm::vec3(0.3f, -1.0f, 1.0f), glm::vec3(0.1f), glm::vec3(0.5f), glm::vec3(0.8f) });
	// Setup cubemap
	std::shared_ptr<Mesh> cubemapMesh = Primitives::generateCube(1);
	Renderer::setCubemap(cubemapMesh, MaterialLoader::load("cubemap"));
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
			cameraControls(cam, window, deltaTime);
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
		gui.drawSelection(selectedInstance);
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