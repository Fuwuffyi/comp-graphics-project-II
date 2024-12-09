#pragma once

#include "Window.hpp"

class Camera;
class Window;
class SceneNode;

namespace CameraControls {
	void cameraControls(Camera& cam, Window& window, const float deltaTime);
	SceneNode*& getSelection();
}
