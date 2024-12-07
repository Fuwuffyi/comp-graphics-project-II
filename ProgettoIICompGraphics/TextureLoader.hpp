#pragma once

#include <string>

class Texture;

namespace TextureLoader {
	Texture* load(const std::string& textureName, const uint32_t binding, const bool flipImage = false);
	Texture* loadCubemap(const std::string& cubemapDirectory, const uint32_t binding);
	void unload(const std::string& textureName);
	void unloadAll();
}
