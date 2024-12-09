#pragma once

#include <string>
#include <vector>
#include <memory>

class MeshInstanceNode;
class Transform;
class SceneNode;
class Material;
class Mesh;

namespace MeshLoader {
	std::shared_ptr<SceneNode> loadMesh(const std::string& fileName, const Transform& rootTransform, const std::vector<std::shared_ptr<Material>>& materialOverrides = std::vector<std::shared_ptr<Material>>());
}