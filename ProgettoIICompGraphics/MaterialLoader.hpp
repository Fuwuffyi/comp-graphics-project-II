#pragma once

#include <string>

class Material;

namespace MaterialLoader {
	Material* load(const std::string& materialAssetFileName);
	Material* get(const std::string& materialName);
	void unload(const std::string& materialName);
	void unloadAll();
}
