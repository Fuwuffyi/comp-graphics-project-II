#include "MaterialLoader.hpp"

#include "ShaderLoader.hpp"
#include "Material.hpp"

#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <fstream>

static constexpr std::string_view MATERIAL_ASSET_DIR = "assets/materials/";
static constexpr std::string_view MATERIAL_ASSET_FILE_EXTENSION = ".material";

namespace MaterialLoader {
	static std::unordered_map<std::string, std::pair<uint32_t, Material>> loadedMaterials;

	static std::tuple<std::string, bool, bool, std::unordered_map<std::string, Material::MaterialValueType>> readMaterialAssetFile(const std::string& materialFile);
}

std::tuple<std::string, bool, bool, std::unordered_map<std::string, Material::MaterialValueType>> MaterialLoader::readMaterialAssetFile(const std::string& materialFile) {
	// TODO: read file format
}

Material* MaterialLoader::load(const std::string& materialAssetFileName) {
	if (materialAssetFileName.empty()) {
		return nullptr;
	}
	auto [shaderName, transparent, lit, propertyMap] = readMaterialAssetFile(materialAssetFileName + std::string(MATERIAL_ASSET_FILE_EXTENSION));
	Shader* shader = ShaderLoader::get(shaderName);
	if (shader == nullptr) {
		ShaderLoader::load(shaderName);
	}
	if (loadedMaterials.find(materialAssetFileName) == loadedMaterials.end()) {
		loadedMaterials.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(materialAssetFileName),
			std::forward_as_tuple(
				std::piecewise_construct,
				std::forward_as_tuple(0u),
				std::forward_as_tuple(materialAssetFileName, shader, propertyMap, lit, transparent)
			)
		);
	}
	return get(materialAssetFileName);
}

Material* MaterialLoader::get(const std::string& materialName) {
	if (loadedMaterials.find(materialName) == loadedMaterials.end()) {
		return nullptr;
	}
	++loadedMaterials.at(materialName).first;
	return &loadedMaterials.at(materialName).second;
}

void MaterialLoader::unload(const std::string& materialName) {
	if (loadedMaterials.find(materialName) == loadedMaterials.end()) {
		return;
	}
	uint32_t& refCount = loadedMaterials.at(materialName).first;
	if (--refCount == 0u) {
		loadedMaterials.erase(materialName);
	}
}

void MaterialLoader::unloadAll() {
	loadedMaterials.clear();
}
