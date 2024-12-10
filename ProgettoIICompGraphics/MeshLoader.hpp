#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class MeshInstanceNode;
class Transform;
class SceneNode;
class Material;
class Mesh;

namespace MeshLoader {
	std::shared_ptr<SceneNode> loadMesh(const std::string& fileName, const Transform& rootTransform, const std::unordered_map<uint32_t, std::shared_ptr<Material>>& materialOverrides = std::unordered_map<uint32_t, std::shared_ptr<Material>>());
}