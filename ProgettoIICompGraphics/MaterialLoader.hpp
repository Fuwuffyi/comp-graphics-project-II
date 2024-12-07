#pragma once

#include <string>
#include <memory>

class Material;

namespace MaterialLoader {
	std::shared_ptr<Material> load(const std::string& materialAssetFileName);
	void unloadAll();
}
