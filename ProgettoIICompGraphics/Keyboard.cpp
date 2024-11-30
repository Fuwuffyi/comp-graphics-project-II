#include "Keyboard.hpp"

bool Keyboard::keys[GLFW_KEY_LAST] = { false };
bool Keyboard::keysChanged[GLFW_KEY_LAST] = { false };

void Keyboard::keyCallback(GLFWwindow* window, const int32_t key, const int32_t scancode, const int32_t action, const int32_t mods) {
	if (action != GLFW_RELEASE) {
		if (!Keyboard::keys[key]) {
			Keyboard::keys[key] = true;
		}
	}
	else {
		Keyboard::keys[key] = false;
	}
	Keyboard::keysChanged[key] = action != GLFW_REPEAT;
}

bool Keyboard::key(const int32_t key) {
	return Keyboard::keys[key];
}

bool Keyboard::keyChanged(const int32_t key) {
	const bool ret = Keyboard::keysChanged[key];
	Keyboard::keysChanged[key] = false;
	return ret;
}

bool Keyboard::keyWentUp(const int32_t key) {
	return !Keyboard::keys[key] && Keyboard::keyChanged(key);
}

bool Keyboard::keyWentDown(const int32_t key) {
	return Keyboard::keys[key] && Keyboard::keyChanged(key);
}