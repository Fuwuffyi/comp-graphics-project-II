#pragma once

#include "Material.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace MaterialLoader {
	std::shared_ptr<Material> load(const std::string& materialAssetFileName);
	std::shared_ptr<Material> load(const std::string& name, const std::string& shaderName, const std::unordered_map<std::string, Material::MaterialValueType>& properties = std::unordered_map<std::string, Material::MaterialValueType>(), const std::unordered_map<std::string, std::shared_ptr<Texture>>& textures = std::unordered_map<std::string, std::shared_ptr<Texture>>());
	void unloadAll();

	bool isLoaded(const std::string& materialAssetFileName);
	std::vector<std::string> getAllFileNames();
}
