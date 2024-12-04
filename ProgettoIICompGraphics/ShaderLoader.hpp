#pragma once

#include <string>

class Shader;

namespace ShaderLoader {
	Shader* load(const std::string& name, const std::string& vertShaderFile = "", const std::string& fragShaderFile = "");
	void unload(const std::string& name);
	void unloadAll();
}
