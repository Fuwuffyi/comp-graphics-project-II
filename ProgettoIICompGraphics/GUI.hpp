#pragma once

struct GLFWwindow;
class MeshInstance;

class GUI {
public:
	GUI(GLFWwindow* window);
	~GUI();

	bool clickedOnUi() const;
	void newFrame() const;
	void drawSelection(MeshInstance* selectedObject) const;
	void drawResources() const;
	void endRendering() const;
};
