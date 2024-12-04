#include "ShaderLoader.hpp"

#include "Shader.hpp"

#include <unordered_map>
#include <stdexcept>
#include <fstream>

namespace ShaderLoader {
	static std::unordered_map<std::string, std::pair<uint32_t, Shader>> loadedShaders;

	static std::string readShaderSource(const std::string& shaderFile);
}

std::string ShaderLoader::readShaderSource(const std::string& shaderFile) {
	std::ifstream file(shaderFile, std::ios::binary);
	if (!file) {
		throw std::runtime_error("Failed to open shader file");
	}
	file.seekg(0, std::ios::end);
	const std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::string contents(static_cast<uint64_t>(size), ' ');
	file.read(&contents[0], static_cast<int64_t>(contents.size()));
	return contents;
}

Shader* ShaderLoader::load(const std::string& name, const std::string& vertShader, const std::string& fragShader) {
	if (loadedShaders.find(name) == loadedShaders.end()) {
		loadedShaders.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(
				std::piecewise_construct,
				std::forward_as_tuple(0u),
				std::forward_as_tuple(readShaderSource(vertShader), readShaderSource(fragShader))
			)
		);
	}
	++loadedShaders.at(name).first;
	return &loadedShaders.at(name).second;
}

void ShaderLoader::unload(const std::string& name) {
	uint32_t& refCount = loadedShaders.at(name).first;
	if (--refCount == 0u) {
		loadedShaders.erase(name);
	}
}

void ShaderLoader::unloadAll() {
	loadedShaders.clear();
}
