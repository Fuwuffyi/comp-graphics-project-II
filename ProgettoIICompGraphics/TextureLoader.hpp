#pragma once

#include <string>
#include <memory>

class Texture;

namespace TextureLoader {
	std::shared_ptr<Texture> load(const std::string& textureName, const uint32_t binding, const bool flipImage = false);
	std::shared_ptr<Texture> loadCubemap(const std::string& cubemapDirectory, const uint32_t binding);
	void unloadAll();
}
