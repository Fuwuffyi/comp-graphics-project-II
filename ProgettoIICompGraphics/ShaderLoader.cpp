#include "ShaderLoader.hpp"

#include "Shader.hpp"

#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <fstream>

static constexpr std::string_view SHADER_ASSET_DIR = "assets/shaders/";
static constexpr std::string_view SHADER_ASSET_FILE_EXTENSION = ".shader";
static constexpr std::string_view SHADER_SOURCE_DIR = "assets/shaders/sources/";

namespace ShaderLoader {
	static std::unordered_map<std::string, std::pair<uint32_t, Shader>> loadedShaders;

	static std::string readShaderSource(const std::string& shaderFile);
	static std::tuple<std::string, std::string> readShaderAssetFile(const std::string& shaderAssetFile);
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
	return contents;
}

std::tuple<std::string, std::string> ShaderLoader::readShaderAssetFile(const std::string& shaderAssetFile) {
	// Open shader asset file
	std::ifstream assetFile(std::string(SHADER_ASSET_DIR) + shaderAssetFile);
	if (!assetFile.is_open()) {
		throw std::runtime_error("Failed to open shader asset file: " + shaderAssetFile);
	}
	// Prepare variables to output
	std::string line, vertShaderFile, fragShaderFile;
	while (std::getline(assetFile, line)) {
		// Read all lines
		std::istringstream iss(line);
		std::string key, value;
		// Get the key value pairs of the shader file
		iss >> key >> value;
		// Check the indices
		if (key == "vertex") {
			vertShaderFile = std::string(SHADER_SOURCE_DIR) + value;
		} else if (key == "fragment") {
			fragShaderFile = std::string(SHADER_SOURCE_DIR) + value;
		}
	}
	// Exit if one of them is not present
	if (vertShaderFile.empty() || fragShaderFile.empty()) {
		throw std::runtime_error("Shader asset file missing required fields: " + shaderAssetFile);
	}
	// Return the values
	return { vertShaderFile, fragShaderFile };
}

Shader* ShaderLoader::load(const std::string& shaderAssetFileName) {
	if (shaderAssetFileName.empty()) {
		return nullptr;
	}
	auto [vertShaderFile, fragShaderFile] = readShaderAssetFile(shaderAssetFileName + std::string(SHADER_ASSET_FILE_EXTENSION));
	if (loadedShaders.find(shaderAssetFileName) == loadedShaders.end()) {
		loadedShaders.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(shaderAssetFileName),
			std::forward_as_tuple(
				std::piecewise_construct,
				std::forward_as_tuple(0u),
				std::forward_as_tuple(shaderAssetFileName, readShaderSource(vertShaderFile), readShaderSource(fragShaderFile))
			)
		);
	}
	return get(shaderAssetFileName);
}

Shader* ShaderLoader::get(const std::string& shaderName) {
	if (loadedShaders.find(shaderName) == loadedShaders.end()) {
		return nullptr;
	}
	++loadedShaders.at(shaderName).first;
	return &loadedShaders.at(shaderName).second;
}

void ShaderLoader::unload(const std::string& shaderName) {
	if (loadedShaders.find(shaderName) == loadedShaders.end()) {
		return;
	}
	uint32_t& refCount = loadedShaders.at(shaderName).first;
	if (--refCount == 0u) {
		loadedShaders.erase(shaderName);
	}
}

void ShaderLoader::unloadAll() {
	loadedShaders.clear();
}
