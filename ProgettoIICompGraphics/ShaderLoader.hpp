#pragma once

#include <memory>
#include <string>
#include <vector>

class Shader;

namespace ShaderLoader {
	std::shared_ptr<Shader> load(const std::string& shaderAssetFileName);
	void unloadAll();

	bool isLoaded(const std::string& shaderAssetFileName);
	std::vector<std::string> getAllFileNames();
}
