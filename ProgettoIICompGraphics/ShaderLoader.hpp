#pragma once

#include <string>

class Shader;

namespace ShaderLoader {
	Shader* load(const std::string& shaderAssetFileName);
	Shader* get(const std::string& shaderName);
	void unload(const std::string& shaderName);
	void unloadAll();
}
