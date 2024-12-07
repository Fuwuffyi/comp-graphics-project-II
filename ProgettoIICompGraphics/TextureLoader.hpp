#pragma once

#include <string>
#include <memory>
#include <vector>

class Texture;

namespace TextureLoader {
	std::shared_ptr<Texture> load(const std::string& textureName, const bool flipImage = false);
	std::shared_ptr<Texture> loadCubemap(const std::string& cubemapDirectory);
	void unloadAll();

	bool isLoaded(const std::string& textureName);
	std::vector<std::string> getAllTextureNames();
	std::vector<std::string> getAllCubemapNames();
}
