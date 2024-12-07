#pragma once

struct GLFWwindow;

class GUI {
public:
	GUI(GLFWwindow* window);
	~GUI();

	bool clickedOnUi() const;
	void newFrame() const;
	void render() const;
};
