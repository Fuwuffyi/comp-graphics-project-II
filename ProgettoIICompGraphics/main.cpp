#include "Window.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "ShaderLoader.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Material.hpp"

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
	Camera cam(Transform(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 180.0f, 0.0f)), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 1.0f, 0.1f, 100.0f);
	// Testing mesh
	std::vector<Vertex3D> verts = {
		Vertex3D {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f) },
		Vertex3D {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f) },
		Vertex3D {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f) },
		Vertex3D {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f) },
		Vertex3D {glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f) }
	};
	std::vector<uint32_t> inds = {
		0, 2, 1,
		0, 3, 2,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};
	const Mesh3D m(verts, inds, GL_TRIANGLES);
	Shader* testingShader = ShaderLoader::load("testing");
	Transform testTransform0(glm::vec3(-0.2f, -0.3f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	Transform testTransform1(glm::vec3(0.1f, 0.4f, 0.0f), glm::vec3(0.0f, 45.0f, 0.0f), glm::vec3(0.5f));
	Transform testTransform2(glm::vec3(0.3f, -0.6f, 0.0f), glm::vec3(45.0f, 0.0f, 0.0f), glm::vec3(0.5f));
	Transform testTransform3(glm::vec3(-0.5f, 0.2f, 0.0f), glm::vec3(0.0f, 0.0f, 45.0f), glm::vec3(0.5f));
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
		testTransform0.setRotation(testTransform0.getRotation() + glm::vec3(0.3f, 1.0f, 0.2f) * deltaTime * 100.0f);
		testTransform1.setRotation(testTransform1.getRotation() + glm::vec3(0.3f, 1.0f, 0.2f) * deltaTime * 100.0f);
		testTransform2.setRotation(testTransform2.getRotation() + glm::vec3(0.3f, 1.0f, 0.2f) * deltaTime * 100.0f);
		testTransform3.setRotation(testTransform3.getRotation() + glm::vec3(0.3f, 1.0f, 0.2f) * deltaTime * 100.0f);
		// Clear buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Test draw
		testingShader->activate();
		testingShader->setUniform("materialColor", glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
		testingShader->setUniformMatrix("cameraMatrix", cam.getCameraMatrix());
		testingShader->setUniformMatrix("objMatrix", testTransform0.getTransformMatrix());
		m.draw();
		testingShader->setUniformMatrix("objMatrix", testTransform1.getTransformMatrix());
		m.draw();
		testingShader->setUniformMatrix("objMatrix", testTransform2.getTransformMatrix());
		m.draw();
		testingShader->setUniformMatrix("objMatrix", testTransform3.getTransformMatrix());
		m.draw();
		// End frame
		window.swapBuffers();
		glfwPollEvents();
	}
	ShaderLoader::unloadAll();
	return EXIT_SUCCESS;
}