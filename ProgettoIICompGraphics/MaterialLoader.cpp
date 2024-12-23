#include "MaterialLoader.hpp"

#include "Shader.hpp"
#include "ShaderLoader.hpp"
#include "TextureLoader.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace MaterialLoader {
	static std::unordered_map<std::string, std::shared_ptr<Material>> loadedMaterials;

	static constexpr const char* MATERIAL_ASSET_DIR = "assets/materials/";
	static constexpr const char* MATERIAL_ASSET_FILE_EXTENSION = ".material";

	static constexpr const char* SHADER_KEY = "shader";
	static constexpr const char* LIT_KEY = "lit";
	static constexpr const char* TRANSPARENT_KEY = "transparent";
	static constexpr const char* PROPERTY_KEY = "p";
	static constexpr const char* TEXTURE_PROPERTY_KEY = "t";

	static std::tuple<std::string, std::unordered_map<std::string, Material::MaterialValueType>, std::unordered_map<std::string, std::shared_ptr<Texture>>, bool, bool> readMaterialAssetFile(const std::string& materialAssetFile);
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

std::tuple<std::string, std::unordered_map<std::string, Material::MaterialValueType>, std::unordered_map<std::string, std::shared_ptr<Texture>>, bool, bool> MaterialLoader::readMaterialAssetFile(const std::string& materialAssetFile) {
	// Open shader asset file
	std::ifstream assetFile(MATERIAL_ASSET_DIR + materialAssetFile);
	if (!assetFile.is_open()) {
		throw std::runtime_error("Failed to open material asset file: " + materialAssetFile);
	}
	// Prepare variables to output
	std::unordered_map<std::string, Material::MaterialValueType> materialProperties;
	std::unordered_map<std::string, std::shared_ptr<Texture>> materialTextures;
	std::string line, shaderText, litText, transparentText;
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
		} else if (key == TEXTURE_PROPERTY_KEY) {
			std::string propertyName, type;
			iss >> propertyName >> type >> value;
			if (type == "cubemap") {
				materialTextures[propertyName] = TextureLoader::loadCubemap(value);
			} else if (type == "texture2D") {
				materialTextures[propertyName] = TextureLoader::load(value);
			}
		} else {
			iss >> value;
			// Check the indices
			if (key == SHADER_KEY) {
				shaderText = value;
			} else if (key == LIT_KEY) {
				litText = value;
			} else if (key == TRANSPARENT_KEY) {
				transparentText = value;
			}
		}
	}
	assetFile.close();
	// Exit if one of them is not present
	if (shaderText.empty()) {
		throw std::runtime_error("Missing shader in material asset: " + materialAssetFile);
	}
	if (litText.empty()) {
		throw std::runtime_error("Missing lit property in material asset: " + materialAssetFile);
	}
	if (transparentText.empty()) {
		throw std::runtime_error("Missing transparent property in material asset: " + materialAssetFile);
	}
	// Return the values
	return { shaderText, materialProperties, materialTextures, static_cast<bool>(std::stoi(litText)), static_cast<bool>(std::stoi(transparentText)) };
}

std::shared_ptr<Material> MaterialLoader::load(const std::string& materialAssetFileName) {
	// If no material, return a null pointer
	if (materialAssetFileName.empty()) {
		return nullptr;
	}
	// If already loaded, return reference of it
	if (isLoaded(materialAssetFileName)) {
		return loadedMaterials.at(materialAssetFileName);
	}
	std::cout << "Loaded Material: " << materialAssetFileName << std::endl;
	// Read the file
	auto [shaderName, propertyMap, textureMap, litFlag, transparentFlag] = readMaterialAssetFile(materialAssetFileName + MATERIAL_ASSET_FILE_EXTENSION);
	// Load the material
	loadedMaterials.emplace(materialAssetFileName, std::make_shared<Material>(materialAssetFileName, ShaderLoader::load(shaderName), propertyMap, textureMap, litFlag, transparentFlag));
	return loadedMaterials.at(materialAssetFileName);
}

std::shared_ptr<Material> MaterialLoader::load(const std::string& name, const std::string& shaderName, const std::unordered_map<std::string, Material::MaterialValueType>& properties, const std::unordered_map<std::string, std::shared_ptr<Texture>>& textures, const bool litFlag, const bool transparentFlag) {
	if (isLoaded(name)) {
		return loadedMaterials.at(name);
	}
	loadedMaterials.emplace(name, std::make_shared<Material>(name, ShaderLoader::load(shaderName), properties, textures, litFlag, transparentFlag));
	return loadedMaterials.at(name);
}

void MaterialLoader::unloadAll() {
	loadedMaterials.clear();
}

bool MaterialLoader::isLoaded(const std::string& materialAssetFileName) {
	return loadedMaterials.find(materialAssetFileName) != loadedMaterials.end();
}

std::vector<std::string> MaterialLoader::getAllFileNames() {
	std::vector<std::string> names;
	for (const auto& entry : std::filesystem::directory_iterator(MATERIAL_ASSET_DIR)) {
		if (std::filesystem::is_regular_file(entry)) {
			names.emplace_back(entry.path().stem().string());
		}
	}
	for (auto mat : loadedMaterials) {
		const std::string name = mat.second->name;
		auto it = std::find(names.begin(), names.end(), name);
		if (it == names.end()) {
			names.emplace_back(name);
		}
	}
	return names;
}
