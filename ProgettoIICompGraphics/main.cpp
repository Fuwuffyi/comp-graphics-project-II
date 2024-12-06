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
	const std::vector<Mesh3D *> meshes = MeshLoader::loadMesh("assets/meshes/dragon_vrip.ply");
	Material* dragonMaterial = MaterialLoader::load("testing");
	Shader* dragonShader = ShaderLoader::load("testing");
	Transform dragonTrasnformA(glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.0f));
	Transform dragonTrasnformB(glm::vec3(0.0f, 0.0f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
	Transform dragonTrasnformC(glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(1.0f));
	Transform dragonTrasnformD(glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 270.0f, 0.0f), glm::vec3(1.0f));
	Transform dragonsTransform(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(6.0f));
	MeshInstance3D instanceA(meshes[0], dragonMaterial, dragonTrasnformA);
	MeshInstance3D instanceB(meshes[0], dragonMaterial, dragonTrasnformB);
	MeshInstance3D instanceC(meshes[0], dragonMaterial, dragonTrasnformC);
	MeshInstance3D instanceD(meshes[0], dragonMaterial, dragonTrasnformD);
	ModelInstance3D instance(std::vector<MeshInstance3D>({ instanceA, instanceB, instanceC, instanceD }), dragonsTransform);
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
		instance.getMutableTransform().setRotation(glm::vec3(0.0f, 1.0f, 0.0f) * (float)glfwGetTime() * 300.0f);
		// Clear buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Test draw
		auto drawables = instance.getDrawables();
		for (auto [meshPtr, materialPtr, model] : drawables) {
			materialPtr->activate();
			dragonShader->setUniformMatrix("cameraMatrix", cam.getCameraMatrix());
			dragonShader->setUniformMatrix("objMatrix", model);
			meshPtr->draw();
		}
		// End frame
		window.swapBuffers();
		glfwPollEvents();
	}
	for (const Mesh3D* m : meshes) {
		delete m;
	}
	MaterialLoader::unloadAll();
	ShaderLoader::unloadAll();
	return EXIT_SUCCESS;
}