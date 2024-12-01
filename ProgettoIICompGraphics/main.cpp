#include "Window.hpp"
#include "Vertex.hpp"
#include "Mesh2D.hpp"
#include "Shader.hpp"

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
	// Testing mesh
	std::vector<Vertex2D> verts = {
		Vertex2D {glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f), glm::vec2(0.0f) },
		Vertex2D {glm::vec2(0.5f, -0.5f), glm::vec2(0.0f), glm::vec2(0.0f) },
		Vertex2D {glm::vec2(0.0f, 0.5f), glm::vec2(0.0f), glm::vec2(0.0f) }
	};
	std::vector<uint32_t> inds = {
		0, 1, 2
	};
	const Mesh2D m(verts, inds, GL_TRIANGLES);
	const Shader testingShader("testing.frag.glsl", "testing.vert.glsl");
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
		// Clear buffers
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Test draw
		testingShader.activate();
		m.draw();
		// End frame
		window.swapBuffers();
		glfwPollEvents();
	}
	return EXIT_SUCCESS;
}