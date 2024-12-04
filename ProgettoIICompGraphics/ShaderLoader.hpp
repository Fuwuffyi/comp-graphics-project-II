#pragma once

#include <string>

class Shader;

namespace ShaderLoader {
	Shader* load(const std::string& name, const std::string& vertShader, const std::string& fragShader);
	void unload(const std::string& name);
	void unloadAll();
}
