#pragma once

#include <string>

class Shader;

namespace ShaderLoader {
	Shader* load(const std::string& shaderAssetFileName);
	void unload(const std::string& shaderName);
	void unloadAll();
}
