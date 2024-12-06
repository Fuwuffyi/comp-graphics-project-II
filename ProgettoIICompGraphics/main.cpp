#include "Window.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "ShaderLoader.hpp"
#include "MaterialLoader.hpp"
#include "MeshInstance.hpp"
#include "ModelInstance.hpp"
#include "MeshLoader.hpp"
#include "RenderingQueue.hpp"

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
	const std::string windowName = "unnamed window";
	Window window(windowName, glm::uvec2(900, 900));
	window.setWindowActive();
	// Testing camera
	Camera cam(Transform(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 180.0f, 0.0f)), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 1.0f, 0.1f, 1000.0f);
	// Testing mesh
	const std::vector<Mesh *> meshes = MeshLoader::loadMesh("assets/meshes/dragon_vrip.ply");
	Material* dragonMaterial = MaterialLoader::load("testing");
	Shader* dragonShader = ShaderLoader::load("testing");
	Transform dragonTrasnformA(glm::vec3(0.0f, 0.0f, 0.25f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.0f));
	Transform dragonTrasnformB(glm::vec3(0.0f, 0.0f, -0.25f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
	Transform dragonTrasnformC(glm::vec3(0.25f, 0.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(1.0f));
	Transform dragonTrasnformD(glm::vec3(-0.25f, 0.0f, 0.0f), glm::vec3(0.0f, 270.0f, 0.0f), glm::vec3(1.0f));
	Transform dragonsTransform(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(6.0f));
	MeshInstance instanceA(meshes[0], dragonMaterial, dragonTrasnformA);
	MeshInstance instanceB(meshes[0], dragonMaterial, dragonTrasnformB);
	MeshInstance instanceC(meshes[0], dragonMaterial, dragonTrasnformC);
	MeshInstance instanceD(meshes[0], dragonMaterial, dragonTrasnformD);
	ModelInstance instance(std::vector<MeshInstance>({ instanceA, instanceB, instanceC, instanceD }), dragonsTransform);
	RenderingQueue queue;
	queue.addRenderable(&instance);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// Cull back faces (!!! ONLY COUNTER CLOCKWISE FACES ARE RENDERED !!!)
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
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
		cameraControls(cam, window, deltaTime);
		// Dragons rotation stuff
		instance.getMutableTransform().setRotation(glm::vec3(0.0f, 1.0f, 0.0f) * (float)glfwGetTime() * 100.0f);
		// Clear buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Test draw
		queue.render(cam.getCameraMatrix(), cam.getTransform().getPosition());
		// End frame
		window.swapBuffers();
		glfwPollEvents();
	}
	for (const Mesh* m : meshes) {
		delete m;
	}
	MaterialLoader::unloadAll();
	ShaderLoader::unloadAll();
	return EXIT_SUCCESS;
}