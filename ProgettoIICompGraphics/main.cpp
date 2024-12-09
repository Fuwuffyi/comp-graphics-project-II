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
#include <iostream>

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
	// Test out primitives
	std::shared_ptr<SceneNode> scene = std::make_shared<SceneNode>("Scene", Transform());
	
	Mesh* planePrimitive = Primitives::generatePlane(5);
	Transform planeTransform(glm::vec3(0.0f, -0.1f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	std::shared_ptr<MeshInstanceNode> planeInstance = std::make_shared<MeshInstanceNode>("Planius maximus", planePrimitive, MaterialLoader::load("blinn_phong"), planeTransform, scene);

	Mesh* cubePrimitive = Primitives::generateCube(25);
	Transform cubeTransform(glm::vec3(0.0f, 0.2f, 1.0f), glm::vec3(0.0f, 45.0f, 0.0f), glm::vec3(0.5f));
	std::shared_ptr<MeshInstanceNode> cubeInstance = std::make_shared<MeshInstanceNode>("Cubus maximus", cubePrimitive, MaterialLoader::load("phong"), cubeTransform, scene);

	Mesh* pyramidPrimitive = Primitives::generatePyramid(5);
	Transform pyramidTransform(glm::vec3(0.0f, 0.2f, -1.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	std::shared_ptr<MeshInstanceNode> pyramidInstance = std::make_shared<MeshInstanceNode>("Pyramidus maximus", pyramidPrimitive, MaterialLoader::load("blinn_phong"), pyramidTransform, scene);

	Mesh* spherePrimitive = Primitives::generateSphere(15);
	Transform sphereTransform(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	std::shared_ptr<MeshInstanceNode> sphereInstance = std::make_shared<MeshInstanceNode>("Spherius maximus", spherePrimitive, MaterialLoader::load("phong"), sphereTransform, scene);

	Mesh* cylinderPrimitive = Primitives::generateCylinder(1.0f, 0.7f, 1.0f, 25, 25);
	Transform cylinderTransform(glm::vec3(1.0f, 0.2f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	std::shared_ptr<MeshInstanceNode> cylinderInstance = std::make_shared<MeshInstanceNode>("Cylinderius maximus", cylinderPrimitive, MaterialLoader::load("blinn_phong"), cylinderTransform, scene);

	Mesh* conePrimitive = Primitives::generateCone(1.0f, 1.0f, 25, 25);
	Transform coneTransform(glm::vec3(-1.0f, 0.2f, 0.0f), glm::vec3(0.0f), glm::vec3(0.2f));
	std::shared_ptr<MeshInstanceNode> coneInstance = std::make_shared<MeshInstanceNode>("Conius maximus", conePrimitive, MaterialLoader::load("phong"), coneTransform, scene);

	Mesh* thorusPrimitive = Primitives::generateThorus(3.0f, 0.2f, 20);
	Transform thorusTransform(glm::vec3(2.0f, 0.2f, 2.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	std::shared_ptr<MeshInstanceNode> thorusInstance = std::make_shared<MeshInstanceNode>("Thorium", thorusPrimitive, MaterialLoader::load("blinn_phong"), thorusTransform, scene);
	// Load the dragon
	const std::vector<Mesh *> dragonMeshes = MeshLoader::loadMesh("assets/meshes/dragon_vrip.ply");
	Transform dragonTrasnform(glm::vec3(0.0f, 0.0f, 0.25f), glm::vec3(0.0f), glm::vec3(2.0f));
	std::shared_ptr<MeshInstanceNode> dragonInstance = std::make_shared<MeshInstanceNode>("Drake", dragonMeshes[0], MaterialLoader::load("phong"), dragonTrasnform, scene);
	// Add children to scene
	scene->addChild(planeInstance);
	scene->addChild(cubeInstance);
	scene->addChild(pyramidInstance);
	scene->addChild(sphereInstance);
	scene->addChild(cylinderInstance);
	scene->addChild(coneInstance);
	scene->addChild(thorusInstance);
	scene->addChild(dragonInstance);
	// Initialize light System
	LightSystem::initialize();
	LightSystem::setLight(0, LightSystem::DirectionalLight{ glm::vec3(0.3f, -1.0f, 1.0f), glm::vec3(0.1f), glm::vec3(0.5f), glm::vec3(0.8f) });
	// Setup cubemap
	Mesh* cubemapMesh = Primitives::generateCube(1);
	Renderer::setCubemap(cubemapMesh, MaterialLoader::load("cubemap"));
	// Add objects to rendering queue
	Renderer::addToRenderingQueues(planeInstance.get());
	Renderer::addToRenderingQueues(cubeInstance.get());
	Renderer::addToRenderingQueues(pyramidInstance.get());
	Renderer::addToRenderingQueues(sphereInstance.get());
	Renderer::addToRenderingQueues(cylinderInstance.get());
	Renderer::addToRenderingQueues(coneInstance.get());
	Renderer::addToRenderingQueues(thorusInstance.get());
	Renderer::addToRenderingQueues(dragonInstance.get());
	Renderer::setupOpengl();
	// Setup GUI
	GUI gui(window.getWindowPtr());
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
		gui.newFrame();
		// Test draw
		Renderer::renderAll(cam.getCameraMatrix(), cam.getViewMatrix(), cam.getProjectionMatrix(), cam.getTransform().getPosition());
		// Draw gui
		gui.drawLightsEditor();
		gui.drawResources();
		gui.drawSelection(dragonInstance.get());
		gui.endRendering();
		// End frame
		window.swapBuffers();
		glfwPollEvents();
	}
	delete planePrimitive;
	delete cubePrimitive;
	delete pyramidPrimitive;
	delete spherePrimitive;
	delete cylinderPrimitive;
	delete conePrimitive;
	delete thorusPrimitive;
	delete cubemapMesh;
	for (const Mesh* m : dragonMeshes) {
		delete m;
	}
	MaterialLoader::unloadAll();
	ShaderLoader::unloadAll();
	TextureLoader::unloadAll();
	return EXIT_SUCCESS;
}