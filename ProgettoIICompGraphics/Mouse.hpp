#pragma once

#include <GLFW/glfw3.h>

/**
 * Keeps the information about the mouse's position and properties.
 */
class Mouse {
public:
	/**
	 * GLFW callback for the mouse's position.
	 *
	 * \param window The window the callback is for.
	 * \param _x The mouse's x position.
	 * \param _y The mouse's y position.
	 */
	static void cursorPosCallback(GLFWwindow* window, const double _x, const double _y);

	/**
	 * GLFW callback for the mouse's buttons.
	 *
	 * \param window The window the callback is for.
	 * \param button The button pressed.
	 * \param action The type of action performed on the button.
	 * \param mods Other modifier keys.
	 */
	static void mouseButtonCallback(GLFWwindow* window, const int32_t button, const int32_t action, const int32_t mods);

	/**
	 * GLFW callback for the mouse's scroll wheel.
	 *
	 * \param window The window the callback is for.
	 * \param _dx The amount the scrollwheel was changed by (on the X axis, AKA: trackpad)
	 * \param _dy The amount the scrollwheel was changed by on the Y axis
	 */
	static void mouseWheelCallback(GLFWwindow* window, const double _dx, const double _dy);

	/**
	 * Getter for the mouse's X position.
	 *
	 * \return The mouse's X position.
	 */
	static float getMouseX();

	/**
	 * Getter for the mouse's Y position.
	 *
	 * \return The mouse's Y position.
	 */
	static float getMouseY();

	/**
	 * Getter for the mouse's delta X position.
	 *
	 * \return The mouse's delta X position.
	 */
	static float getDx();

	/**
	 * Getter for the mouse's delta Y position.
	 *
	 * \return The mouse's delta Y position.
	 */
	static float getDy();

	/**
	 * Getter for the mouse's scroll X amount.
	 *
	 * \return The mouse's scroll X amount.
	 */
	static float getScrollDx();

	/**
	 * Getter for the mouse's scroll Y amount.
	 *
	 * \return The mouse's scroll Y amount.
	 */
	static float getScrollDy();

	/**
	 * Checks if a button is currently pressed.
	 *
	 * \param button The button to check.
	 * \return True if the button is currently pressed down.
	 */
	static bool button(const int32_t button);

	/**
	 * Checks if a button has changed state.
	 *
	 * \param button The button to check.
	 * \return True if the button just changed state.
	 */
	static bool buttonChanged(const int32_t button);

	/**
	 * Checks if a button has just got pressed.
	 *
	 * \param button The button to check.
	 * \return True if the button just got pressed.
	 */
	static bool buttonWentUp(const int32_t button);

	/**
	 * Checks if a button has just got released.
	 *
	 * \param button The button to check.
	 * \return True if the button just got released.
	 */
	static bool buttonWentDown(const int32_t button);

private:
	/**
	 * Private constructor as it is not used.
	 *
	 */
	Mouse();

	static float x;
	static float y;
	static float lastX;
	static float lastY;
	static float dx;
	static float dy;
	static float scrollDx;
	static float scrollDy;
	static bool firstMouse;
	static bool buttons[];
	static bool buttonsChanged[];
};
