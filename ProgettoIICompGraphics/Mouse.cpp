#include "Mouse.hpp"

float Mouse::x = 0.0f;
float Mouse::y = 0.0f;
float Mouse::lastX = 0.0f;
float Mouse::lastY = 0.0f;
float Mouse::dx = 0.0f;
float Mouse::dy = 0.0f;
float Mouse::scrollDx = 0.0f;
float Mouse::scrollDy = 0.0f;
bool Mouse::firstMouse = true;
bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { false };
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = { false };

void Mouse::cursorPosCallback(GLFWwindow* window, const double _x, const double _y) {
	Mouse::x = static_cast<float>(_x);
	Mouse::y = static_cast<float>(_y);
	if (Mouse::firstMouse) {
		Mouse::lastX = Mouse::x;
		Mouse::lastY = Mouse::y;
		Mouse::firstMouse = false;
	}
	Mouse::dx = Mouse::x - Mouse::lastX;
	Mouse::dy = Mouse::lastY - Mouse::y;
	Mouse::lastX = Mouse::x;
	Mouse::lastY = Mouse::y;
}

void Mouse::mouseButtonCallback(GLFWwindow* window, const int32_t button, const int32_t action, const int32_t mods) {
	if (action != GLFW_RELEASE) {
		if (!Mouse::buttons[button]) {
			Mouse::buttons[button] = true;
		}
	}
	else {
		Mouse::buttons[button] = false;
	}
	Mouse::buttonsChanged[button] = action != GLFW_REPEAT;
}

void Mouse::mouseWheelCallback(GLFWwindow* window, const double _dx, const double _dy) {
	Mouse::scrollDx = static_cast<float>(_dx);
	Mouse::scrollDy = static_cast<float>(_dy);
}

float Mouse::getMouseX() {
	return Mouse::x;
}

float Mouse::getMouseY() {
	return Mouse::y;
}

float Mouse::getDx() {
	const float _dx = Mouse::dx;
	Mouse::dx = 0.0;
	return _dx;
}

float Mouse::getDy() {
	const float _dy = Mouse::dy;
	Mouse::dy = 0.0;
	return _dy;
}

float Mouse::getScrollDx() {
	const float _dx = Mouse::scrollDx;
	Mouse::scrollDx = 0.0;
	return _dx;
}

float Mouse::getScrollDy() {
	const float _dy = Mouse::scrollDy;
	Mouse::scrollDy = 0.0;
	return _dy;
}

bool Mouse::button(const int32_t button) {
	return Mouse::buttons[button];
}

bool Mouse::buttonChanged(const int32_t button) {
	const bool ret = Mouse::buttonsChanged[button];
	Mouse::buttonsChanged[button] = false;
	return ret;
}

bool Mouse::buttonWentUp(const int32_t button) {
	return !Mouse::buttons[button] && Mouse::buttonChanged(button);
}

bool Mouse::buttonWentDown(const int32_t button) {
	return Mouse::buttons[button] && Mouse::buttonChanged(button);
}