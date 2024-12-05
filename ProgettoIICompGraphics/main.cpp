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

#include "AssimpTest.hpp"

#include <glm/gtc/type_ptr.hpp>

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
	const std::vector<Mesh3D *> meshes = importModel("assets/meshes/dragon_vrip.ply");
	Material* dragonMaterial = MaterialLoader::load("testing");
	Shader* dragonShader = ShaderLoader::load("testing");
	Transform dragonTrasnform(glm::vec3(0.0f, -1.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(14.0f));
	MeshInstance3D instance(meshes[0], dragonMaterial, dragonTrasnform);
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
		window.setTitle(windowName + " - " + std::to_string(1.0f / deltaTime) + " FPS");
		// Clear buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Test draw
		auto drawables = instance.getDrawables();
		for (auto [meshPtr, materialPtr, transform] : drawables) {
			materialPtr->activate();
			dragonShader->setUniformMatrix("cameraMatrix", cam.getCameraMatrix());
			dragonShader->setUniformMatrix("objMatrix", transform);
			meshPtr->draw();
		}
		// End frame
		window.swapBuffers();
		glfwPollEvents();
	}
	MaterialLoader::unloadAll();
	ShaderLoader::unloadAll();
	for (const Mesh3D* m : meshes) {
		delete m;
	}
	return EXIT_SUCCESS;
}