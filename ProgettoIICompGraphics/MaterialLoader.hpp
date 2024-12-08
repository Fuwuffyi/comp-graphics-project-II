#pragma once

#include <memory>
#include <string>
#include <vector>

class Material;

namespace MaterialLoader {
	std::shared_ptr<Material> load(const std::string& materialAssetFileName);
	void unloadAll();

	bool isLoaded(const std::string& materialAssetFileName);
	std::vector<std::string> getAllFileNames();
}
