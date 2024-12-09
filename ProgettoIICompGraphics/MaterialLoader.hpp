#pragma once

#include <memory>
#include <string>
#include <vector>

class Material;

namespace MaterialLoader {
	std::shared_ptr<Material> load(const std::string& materialAssetFileName);
	std::shared_ptr<Material> load(const std::string& name, const std::shared_ptr<Material>& loadedMaterial);
	void unloadAll();

	bool isLoaded(const std::string& materialAssetFileName);
	std::vector<std::string> getAllFileNames();
}
