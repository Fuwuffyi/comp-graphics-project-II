#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

/**
 * Forward declaration of the glfw window pointer.
 */
struct GLFWwindow;

/**
 * Class that rapresents a glfw window.
 */
class Window {
private:
	/**
	 * Callback for when the window gets resized.
	 *
	 * \param window The current window.
	 * \param newWidth The new window's width.
	 * \param newHeight The new window's height.
	 */
	static void framebufferSizeCallback(GLFWwindow* window, const int32_t newWidth, const int32_t newHeight);

	std::string title;
	glm::uvec2 dimensions;
	GLFWwindow* glWindow;
public:
	/**
	 * Creates a new window.
	 *
	 * \param _title The window's title.
	 * \param _dimensions The window's starting size.
	 * \param resizable Flag to check if the window should be resizable.
	 */
	Window(const std::string& _title, const glm::uvec2& _dimensions, const bool resizable = true);

	/**
	 * Deallocates the memory allocated by GLFW.
	 */
	~Window();

	/**
	 * Getter for the window's size.
	 *
	 * \return The window's dimensions.
	 */
	const glm::uvec2& getDimensions() const;

	/**
	 * Getter for the window's glfw window pointer.
	 *
	 * \return The glfwWindow pointer.
	 */
	GLFWwindow* getWindowPtr() const;

	/**
	 * Gets the current window's title.
	 *
	 * \return The window's title
	 */
	std::string getTitle() const;

	/**
	 * Sets the window's title.
	 *
	 * \param _title The new window title.
	 */
	void setTitle(const std::string& _title);

	/**
	 * Sets the window as active for drawing.
	 *
	 */
	void setWindowActive() const;

	/**
	 * Checks whether the window should close or not.
	 *
	 * \return True if the window should close.
	 */
	bool shouldClose() const;

	/**
	 * Calls glfw's swap buffers function.
	 *
	 */
	void swapBuffers() const;
private:
	/**
	 * Sets the window's dimensions, called by the callback.
	 *
	 * \param _dimensions The new window's size.
	 */
	void setDimensions(const glm::uvec2& _dimensions);
};