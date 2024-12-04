#include "MaterialLoader.hpp"

#include "ShaderLoader.hpp"
#include "Material.hpp"

#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <fstream>

namespace MaterialLoader {
	static std::unordered_map<std::string, std::pair<uint32_t, Material>> loadedMaterials;

	static constexpr const char* MATERIAL_ASSET_DIR = "assets/materials/";
	static constexpr const char* MATERIAL_ASSET_FILE_EXTENSION = ".material";

	static constexpr const char* SHADER_KEY = "shader";
	static constexpr const char* TRANSPARENT_KEY = "transparent";
	static constexpr const char* PROPERTY_KEY = "p";
	static constexpr const char* LIT_KEY = "lit";

	static std::tuple<std::string, bool, bool, std::unordered_map<std::string, Material::MaterialValueType>> readMaterialAssetFile(const std::string& materialAssetFile);
	static Material::MaterialValueType parseMaterialValue(const std::string& value, const std::string& type);
}

Material::MaterialValueType MaterialLoader::parseMaterialValue(const std::string& value, const std::string& type) {
	if (type == "float") {
		return std::stof(value);
	} else if (type == "vec2") {
		float x, y;
		sscanf_s(value.c_str(), "%f %f", &x, &y);
		return glm::vec2(x, y);
	} else if (type == "vec3") {
		float x, y, z;
		sscanf_s(value.c_str(), "%f %f %f", &x, &y, &z);
		return glm::vec3(x, y, z);
	} else if (type == "vec4") {
		float x, y, z, w;
		sscanf_s(value.c_str(), "%f %f %f %f", &x, &y, &z, &w);
		return glm::vec4(x, y, z, w);
	} else if (type == "ivec2") {
		int x, y;
		sscanf_s(value.c_str(), "%d %d", &x, &y);
		return glm::ivec2(x, y);
	} else if (type == "ivec3") {
		int x, y, z;
		sscanf_s(value.c_str(), "%d %d %d", &x, &y, &z);
		return glm::ivec3(x, y, z);
	} else if (type == "ivec4") {
		int x, y, z, w;
		sscanf_s(value.c_str(), "%d %d %d %d", &x, &y, &z, &w);
		return glm::ivec4(x, y, z, w);
	} else if (type == "uvec2") {
		unsigned int x, y;
		sscanf_s(value.c_str(), "%u %u", &x, &y);
		return glm::uvec2(x, y);
	} else if (type == "uvec3") {
		unsigned int x, y, z;
		sscanf_s(value.c_str(), "%u %u %u", &x, &y, &z);
		return glm::uvec3(x, y, z);
	} else if (type == "uvec4") {
		unsigned int x, y, z, w;
		sscanf_s(value.c_str(), "%u %u %u %u", &x, &y, &z, &w);
		return glm::uvec4(x, y, z, w);
	}
	throw std::invalid_argument("Unknown material property type: " + type);
}

std::tuple<std::string, bool, bool, std::unordered_map<std::string, Material::MaterialValueType>> MaterialLoader::readMaterialAssetFile(const std::string& materialAssetFile) {
	// Open shader asset file
	std::ifstream assetFile(MATERIAL_ASSET_DIR + materialAssetFile);
	if (!assetFile.is_open()) {
		throw std::runtime_error("Failed to open material asset file: " + materialAssetFile);
	}
	// Prepare variables to output
	std::unordered_map<std::string, Material::MaterialValueType> materialProperties;
	std::string line, shaderText, transparentText, litText;
	while (std::getline(assetFile, line)) {
		// Read all lines
		std::istringstream iss(line);
		std::string key, value;
		// Get the key value pairs of the shader file
		iss >> key;
		if (key == PROPERTY_KEY) {
			std::string propertyName, type;
			iss >> propertyName >> type;
			std::getline(iss, value);
			materialProperties[propertyName] = parseMaterialValue(value, type);
		}
		else {
			iss >> value;
			// Check the indices
			if (key == SHADER_KEY) {
				shaderText = value;
			} else if (key == TRANSPARENT_KEY) {
				transparentText = value;
			} else if (key == LIT_KEY) {
				litText = value;
			}
		}
	}
	// Exit if one of them is not present
	if (shaderText.empty()) {
		throw std::runtime_error("Missing shader in material asset: " + materialAssetFile);
	}
	if (transparentText.empty()) {
		throw std::runtime_error("Missing transparency flag in material asset: " + materialAssetFile);
	}
	if (litText.empty()) {
		throw std::runtime_error("Missing lit flag in material asset: " + materialAssetFile);
	}
	// Return the values
	return { shaderText, static_cast<bool>(std::stoi(transparentText)), static_cast<bool>(std::stoi(litText)), materialProperties };
}

Material* MaterialLoader::load(const std::string& materialAssetFileName) {
	// If no material, return a null pointer
	if (materialAssetFileName.empty()) {
		return nullptr;
	}
	// If already loaded, return reference of it
	if (loadedMaterials.find(materialAssetFileName) != loadedMaterials.end()) {
		++loadedMaterials.at(materialAssetFileName).first;
		return &loadedMaterials.at(materialAssetFileName).second;
	}
	// Read the file
	auto [shaderName, transparent, lit, propertyMap] = readMaterialAssetFile(materialAssetFileName + MATERIAL_ASSET_FILE_EXTENSION);
	// Load the shader
	Shader* shader = ShaderLoader::load(shaderName);
	// Load the material
	loadedMaterials.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(materialAssetFileName),
		std::forward_as_tuple(
			std::piecewise_construct,
			std::forward_as_tuple(0u),
			std::forward_as_tuple(materialAssetFileName, shader, propertyMap, lit, transparent)
		)
	);
	return &loadedMaterials.at(materialAssetFileName).second;
}

void MaterialLoader::unload(const std::string& materialName) {
	auto it = loadedMaterials.find(materialName);
	if (it == loadedMaterials.end()) {
		return;
	}
	if (--it->second.first == 0u) {
		loadedMaterials.erase(materialName);
	}
}

void MaterialLoader::unloadAll() {
	loadedMaterials.clear();
}
