#include "Window.hpp"
#include "Vertex.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "ShaderLoader.hpp"
#include "MaterialLoader.hpp"
#include "MeshInstance.hpp"
#include "ModelInstance.hpp"
#include "MeshLoader.hpp"
#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Primitives.hpp"

#include "Keyboard.hpp"
#include "Mouse.hpp"

#include <glm/gtc/type_ptr.hpp>

void cameraControls(Camera& cam, Window& window, const float deltaTime) {
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
	if (Mouse::buttonWentDown(GLFW_MOUSE_BUTTON_LEFT) || Mouse::buttonWentDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
		glfwSetInputMode(window.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	} else if (Mouse::buttonWentUp(GLFW_MOUSE_BUTTON_LEFT) || Mouse::buttonWentUp(GLFW_MOUSE_BUTTON_MIDDLE)) {
		glfwSetInputMode(window.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	// Setup FPS style camera when holding left click
	if (Mouse::button(GLFW_MOUSE_BUTTON_LEFT)) {
		const float mouseDeltaX = Mouse::getDx();
		const float mouseDeltaY = Mouse::getDy();
		cam.getMutableTransform().setRotation(cam.getTransform().getRotation() + glm::vec3(mouseDeltaY, -mouseDeltaX, 0.0f) * sensitivity);
	}
	// Change FOV/Zoom based on scroll wheel
	constexpr const float epsilon = 0.01f;
	static float trackballZoom = 1.0f;
	if (Mouse::button(GLFW_MOUSE_BUTTON_MIDDLE)) {
		const float newZoom = trackballZoom - Mouse::getScrollDy() * 100.0f * deltaTime;
		trackballZoom = std::min(std::max(newZoom, epsilon), 10.0f);
	} else {
		const float newFov = cam.getFOV() - Mouse::getScrollDy() * 1000.0f * deltaTime;
		cam.setFOV(std::min(std::max(newFov, epsilon), 180.0f - epsilon));
	}
	// Setup trackball style camera when holding middle mouse button
	glm::vec3 target;
	if (Mouse::buttonWentDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
		target = cam.getTransform().getPosition() + cam.getViewDirection() * trackballZoom;
	}
	// TODO: Fix trackball movement
	if (Mouse::button(GLFW_MOUSE_BUTTON_MIDDLE)) {
		const float mouseDeltaX = Mouse::getDx();
		const float mouseDeltaY = Mouse::getDy();
		const glm::vec3 currentRotation = cam.getTransform().getRotation();
		const float yaw = currentRotation.y + mouseDeltaX * sensitivity;
		const float pitch = currentRotation.x + mouseDeltaY * sensitivity;
		cam.getMutableTransform().setRotation(glm::vec3(pitch, yaw, currentRotation.z));
		const glm::vec3 newPosition = target - cam.getViewDirection() * trackballZoom;
		cam.getMutableTransform().setPosition(newPosition);
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
	// Load some materials
	Material* materialPhong = MaterialLoader::load("phong");
	Material* materialBlinn = MaterialLoader::load("blinn_phong");
	// Test out primitives
	Mesh* planePrimitive = Primitives::generatePlane(5);
	Transform planeTransform(glm::vec3(0.0f, -0.1f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	MeshInstance planeInstance(planePrimitive, materialBlinn, planeTransform);

	Mesh* cubePrimitive = Primitives::generateCube(25);
	Transform cubeTransform(glm::vec3(0.0f, 0.2f, 1.0f), glm::vec3(0.0f, 45.0f, 0.0f), glm::vec3(0.5f));
	MeshInstance cubeInstance(cubePrimitive, materialPhong, cubeTransform);

	Mesh* pyramidPrimitive = Primitives::generatePyramid(5);
	Transform pyramidTransform(glm::vec3(0.0f, 0.2f, -1.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	MeshInstance pyramidInstance(pyramidPrimitive, materialBlinn, pyramidTransform);

	Mesh* spherePrimitive = Primitives::generateSphere(15);
	Transform sphereTransform(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	MeshInstance sphereInstance(spherePrimitive, materialPhong, sphereTransform);

	Mesh* cylinderPrimitive = Primitives::generateCylinder(1.0f, 0.7f, 20);
	Transform cylinderTransform(glm::vec3(1.0f, 0.2f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	MeshInstance cylinderInstance(cylinderPrimitive, materialBlinn, cylinderTransform);

	Mesh* conePrimitive = Primitives::generateCone(25);
	Transform coneTransform(glm::vec3(-1.0f, 0.2f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	MeshInstance coneInstance(conePrimitive, materialPhong, coneTransform);

	Mesh* thorusPrimitive = Primitives::generateThorus(3.0f, 0.2f, 20);
	Transform thorusTransform(glm::vec3(2.0f, 0.2f, 2.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	MeshInstance thorusInstance(thorusPrimitive, materialBlinn, thorusTransform);
	// Load the dragon
	const std::vector<Mesh *> dragonMeshes = MeshLoader::loadMesh("assets/meshes/dragon_vrip.ply");
	Transform dragonTrasnform(glm::vec3(0.0f, 0.0f, 0.25f), glm::vec3(0.0f), glm::vec3(2.0f));
	MeshInstance dragonInstance(dragonMeshes[0], materialPhong, dragonTrasnform);
	// Add objects to rendering queue
	Renderer::addToRenderingQueues(&planeInstance);
	Renderer::addToRenderingQueues(&cubeInstance);
	Renderer::addToRenderingQueues(&pyramidInstance);
	Renderer::addToRenderingQueues(&sphereInstance);
	Renderer::addToRenderingQueues(&cylinderInstance);
	Renderer::addToRenderingQueues(&coneInstance);
	Renderer::addToRenderingQueues(&thorusInstance);
	Renderer::addToRenderingQueues(&dragonInstance);
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
		cameraControls(cam, window, deltaTime);
		// Clear buffers
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Test draw
		Renderer::renderAll(cam.getCameraMatrix(), cam.getTransform().getPosition());
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
	for (const Mesh* m : dragonMeshes) {
		delete m;
	}
	MaterialLoader::unloadAll();
	ShaderLoader::unloadAll();
	return EXIT_SUCCESS;
}