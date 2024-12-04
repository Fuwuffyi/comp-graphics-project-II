#include "ShaderLoader.hpp"

#include "Shader.hpp"

#include <unordered_map>

namespace ShaderLoader {
	static std::unordered_map<std::string, std::pair<uint32_t, Shader>> loadedShaders;
}

Shader* ShaderLoader::load(const std::string& name, const std::string& vertShader, const std::string& fragShader) {
	if (loadedShaders.find(name) == loadedShaders.end()) {
		loadedShaders.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(
				std::piecewise_construct,
				std::forward_as_tuple(0u),
				std::forward_as_tuple(vertShader, fragShader)
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
