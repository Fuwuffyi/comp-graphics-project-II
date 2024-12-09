#pragma once

#include <string>
#include <vector>
#include <memory>

class MeshInstanceNode;
class Transform;
class SceneNode;
class Mesh;

namespace MeshLoader {
	std::shared_ptr<SceneNode> loadMesh(const std::string& fileName, const Transform& rootTransform);
}