#pragma once

#include <string>

class Material;

namespace MaterialLoader {
	Material* load(const std::string& materialAssetFileName);
	void unload(const std::string& materialName);
	void unloadAll();
}
