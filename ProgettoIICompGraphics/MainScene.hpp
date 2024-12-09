#pragma once

#include "MainScene.hpp"
#include "MaterialLoader.hpp"
#include "MeshInstanceNode.hpp"
#include "Primitives.hpp"
#include "SceneNode.hpp"
#include "Transform.hpp"
#include <memory>

namespace MainScene {
	std::shared_ptr<SceneNode> getScene();
}
