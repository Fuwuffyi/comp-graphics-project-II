#pragma once

#include <string>
#include <memory>

class Shader;

namespace ShaderLoader {
	std::shared_ptr<Shader> load(const std::string& shaderAssetFileName);
	void unloadAll();
}
