#pragma once

#include <GLFW/glfw3.h>

class Keyboard {
public:
    /**
     * GLFW callback for having pressed any key.
     *
     * \param window The window the callback is for.
     * \param key The currently changed key.
     * \param scancode
     * \param action What action was done to said key.
     * \param mods Other button modifiers (shift, alt, etc...)
     */
    static void keyCallback(GLFWwindow* window, const int32_t key, const int32_t scancode, const int32_t action, const int32_t mods);

    /**
     * Checks if a key is currently pressed.
     *
     * \param key The key to check.
     * \return True if the key is currently being pressed.
     */
    static bool key(const int32_t key);

    /**
     * Checks if a key has changed state.
     *
     * \param key The key to check.
     * \return True if the key has just changed state.
     */
    static bool keyChanged(const int32_t key);

    /**
     * Checks if a key has just been pressed.
     *
     * \param key The key to check.
     * \return True if the key has just been pressed.
     */
    static bool keyWentUp(const int32_t key);

    /**
     * Checks if a key has just been released.
     *
     * \param key The key to check.
     * \return True if the key has just been released.
     */
    static bool keyWentDown(const int32_t key);
private:
    /**
     * Private constructor as it is not used.
     *
     */
    Keyboard();

    static bool keys[];
    static bool keysChanged[];
};
