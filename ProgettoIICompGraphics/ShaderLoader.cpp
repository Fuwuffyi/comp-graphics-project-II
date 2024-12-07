#include "ShaderLoader.hpp"

#include "Shader.hpp"

#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <fstream>

namespace ShaderLoader {
	static std::unordered_map<std::string, std::pair<uint32_t, Shader>> loadedShaders;

	static constexpr const char* SHADER_ASSET_DIR = "assets/shaders/";
	static constexpr const char* SHADER_ASSET_FILE_EXTENSION = ".shader";
	static constexpr const char* SHADER_SOURCE_DIR = "assets/shaders/sources/";

	static constexpr const char* VERTEX_KEY = "vertex";
	static constexpr const char* FRAGMENT_KEY = "fragment";
	static constexpr const char* LIT_KEY = "lit";
	static constexpr const char* TRANSPARENT_KEY = "transparent";

	static std::string readShaderSource(const std::string& shaderFile);
	static std::tuple<std::string, std::string, bool, bool> readShaderAssetFile(const std::string& shaderAssetFile);
}

std::string ShaderLoader::readShaderSource(const std::string& shaderFile) {
	// Open the file
	std::ifstream file(shaderFile, std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open shader file: " + shaderFile);
	}
	// Check the size
	file.seekg(0, std::ios::end);
	const std::streamsize size = file.tellg();
	// Go to start
	file.seekg(0, std::ios::beg);
	// Read file
	std::string contents(static_cast<uint64_t>(size), ' ');
	file.read(&contents[0], static_cast<int64_t>(contents.size()));
	file.close();
	return contents;
}

std::tuple<std::string, std::string, bool, bool> ShaderLoader::readShaderAssetFile(const std::string& shaderAssetFile) {
	// Open shader asset file
	std::ifstream assetFile(SHADER_ASSET_DIR + shaderAssetFile);
	if (!assetFile.is_open()) {
		throw std::runtime_error("Failed to open shader asset file: " + shaderAssetFile);
	}
	// Prepare variables to output
	std::string line, vertShaderFile, fragShaderFile, litValue, transparentValue;
	while (std::getline(assetFile, line)) {
		std::istringstream iss(line);
		std::string key, value;
		iss >> key >> value;
		if (key == VERTEX_KEY) {
			vertShaderFile = SHADER_SOURCE_DIR + value;
		} else if (key == FRAGMENT_KEY) {
			fragShaderFile = SHADER_SOURCE_DIR + value;
		} else if (key == LIT_KEY) {
			litValue = value;
		} else if (key == TRANSPARENT_KEY) {
			transparentValue = value;
		}
	}
	assetFile.close();
	// Exit if one of them is not present
	if (vertShaderFile.empty()) {
		throw std::runtime_error("Missing vertex shader file in shader asset: " + shaderAssetFile);
	}
	if (fragShaderFile.empty()) {
		throw std::runtime_error("Missing fragment shader file in shader asset: " + shaderAssetFile);
	}
	if (litValue.empty()) {
		throw std::runtime_error("Missing lit flag file in shader asset: " + shaderAssetFile);
	}
	if (transparentValue.empty()) {
		throw std::runtime_error("Missing transparent flag file in shader asset: " + shaderAssetFile);
	}
	// Return the values
	return { vertShaderFile, fragShaderFile, static_cast<bool>(std::stoi(litValue)), static_cast<bool>(std::stoi(transparentValue)) };
}

Shader* ShaderLoader::load(const std::string& shaderAssetFileName) {
	// Return null pointer if empty
	if (shaderAssetFileName.empty()) {
		return nullptr;
	}
	// If shader already loaded, return ref
	if (loadedShaders.find(shaderAssetFileName) != loadedShaders.end()) {
		++loadedShaders.at(shaderAssetFileName).first;
		return &loadedShaders.at(shaderAssetFileName).second;
	}
	// Read shader file
	auto [vertShaderFile, fragShaderFile, litFlag, transparentFlag] = readShaderAssetFile(shaderAssetFileName + SHADER_ASSET_FILE_EXTENSION);
	// Load it
	if (loadedShaders.find(shaderAssetFileName) == loadedShaders.end()) {
		loadedShaders.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(shaderAssetFileName),
			std::forward_as_tuple(
				std::piecewise_construct,
				std::forward_as_tuple(0u),
				std::forward_as_tuple(shaderAssetFileName, readShaderSource(vertShaderFile), readShaderSource(fragShaderFile), litFlag, transparentFlag)
			)
		);
	}
	return &loadedShaders.at(shaderAssetFileName).second;
}

void ShaderLoader::unload(const std::string& shaderName) {
	auto it = loadedShaders.find(shaderName);
	if (it == loadedShaders.end()) {
		return;
	}
	if (--it->second.first == 0u) {
		loadedShaders.erase(shaderName);
	}
}

void ShaderLoader::unloadAll() {
	loadedShaders.clear();
}
