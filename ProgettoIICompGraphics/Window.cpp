#include "Window.hpp"

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include <iostream>

void Window::framebufferSizeCallback(GLFWwindow* window, const int32_t newWidth, const int32_t newHeight) {
	// Set window's object data on resize
	Window* windowObj = static_cast<Window*>(glfwGetWindowUserPointer(window));
	windowObj->setDimensions(glm::uvec2(static_cast<uint32_t>(newWidth), static_cast<uint32_t>(newHeight)));
	// Change viewport based on new data
	glViewport(0, 0, newWidth, newHeight);
}

Window::Window(const std::string& _title, const glm::uvec2& _dimensions, const bool resizable)
	:
	title(_title.c_str()),
	dimensions(_dimensions),
	glWindow(glfwCreateWindow(dimensions.x, dimensions.y, this->title.c_str(), nullptr, nullptr))
{
	if (!glWindow) {
		std::cerr << "Failed to create the window!" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	// Set resizable
	glfwSetWindowAttrib(this->glWindow, GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);
	// Set the window's owner to this object 
	glfwSetWindowUserPointer(this->glWindow, this);
	// Set window resize callback
	glfwSetFramebufferSizeCallback(this->glWindow, Window::framebufferSizeCallback);
	// Set input callbacks
	glfwSetKeyCallback(this->glWindow, Keyboard::keyCallback);
	glfwSetCursorPosCallback(this->glWindow, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(this->glWindow, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(this->glWindow, Mouse::mouseWheelCallback);
}

Window::~Window() {
	glfwDestroyWindow(this->glWindow);
	glfwTerminate();
}

const glm::uvec2& Window::getDimensions() const {
	return this->dimensions;
}

GLFWwindow* Window::getWindowPtr() const {
	return this->glWindow;
}

std::string Window::getTitle() const {
	return this->title;
}

void Window::setTitle(const std::string& _title) {
	this->title = _title;
	glfwSetWindowTitle(this->glWindow, _title.c_str());
}

void Window::setWindowActive() const {
	glfwMakeContextCurrent(this->glWindow);
	// Load glad function pointers (every time it is set active, as gl version depends on the window)
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cerr << "Failed to load opengl function pointers!" << std::endl;
		glfwDestroyWindow(this->glWindow);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
}

bool Window::shouldClose() const {
	return glfwWindowShouldClose(this->glWindow);
}

void Window::swapBuffers() const {
	glfwSwapBuffers(this->glWindow);
}

void Window::setDimensions(const glm::uvec2& _dimensions) {
	this->dimensions = _dimensions;
}