#include "TextureLoader.hpp"

#include "Texture.hpp"
#include "Texture2D.hpp"
#include "TextureCubemap.hpp"

#include <stb_image.h>
#include <unordered_map>
#include <stdexcept>

namespace TextureLoader {
	static std::unordered_map<std::string, std::shared_ptr<Texture2D>> loadedTextures;
	static std::unordered_map<std::string, std::shared_ptr<TextureCubemap>> loadedCubemaps;

	static std::tuple<int32_t, int32_t, int32_t, int32_t, uint8_t*> loadTextureData(const std::string& file, const bool flip = false);

	static constexpr const char* TEXTURE_ASSET_DIR = "assets/textures/";
}

std::tuple<int32_t, int32_t, int32_t, int32_t, uint8_t*> TextureLoader::loadTextureData(const std::string& file, const bool flip) {
	int32_t widthImage, heightImage, numColCh;
	stbi_set_flip_vertically_on_load(flip);
	uint8_t* bytes = stbi_load(file.c_str(), &widthImage, &heightImage, &numColCh, 0);
	int32_t inFormat, outFormat;
	switch (numColCh) {
		case 4:
			inFormat = GL_RGBA;
			outFormat = GL_RGBA;
			break;
		case 3:
			inFormat = GL_RGB;
			outFormat = GL_RGB;
			break;
		case 1:
			inFormat = GL_RED;
			outFormat = GL_RED;
			break;
		default:
			throw std::runtime_error("Unsupported file format for: " + file);
	}
	return std::make_tuple(widthImage, heightImage, inFormat, outFormat, bytes);
}

std::shared_ptr<Texture> TextureLoader::load(const std::string& textureName, const bool flipImage) {
	// Return null pointer if empty
	if (textureName.empty()) {
		return nullptr;
	}
	// If shader already loaded, return ref
	if (loadedTextures.find(textureName) != loadedTextures.end()) {
		return loadedTextures.at(textureName);
	}
	auto [imageWidth, imageHeight, inFormat, outFormat, data] = loadTextureData(TEXTURE_ASSET_DIR + textureName);
	loadedTextures.emplace(textureName, std::make_shared<Texture2D>(inFormat, outFormat));
	loadedTextures.at(textureName)->uploadData(imageWidth, imageHeight, data);
	stbi_image_free(data);
	return loadedTextures.at(textureName);
}

std::shared_ptr<Texture> TextureLoader::loadCubemap(const std::string& cubemapDirectory) {
	// Return null pointer if empty
	if (cubemapDirectory.empty()) {
		return nullptr;
	}
	// If shader already loaded, return ref
	if (loadedCubemaps.find(cubemapDirectory) != loadedCubemaps.end()) {
		return loadedCubemaps.at(cubemapDirectory);
	}
	loadedCubemaps.emplace(cubemapDirectory, std::make_shared<TextureCubemap>());
	// Load faces of cubemap
	for (uint32_t i = 0; i < 6; ++i) {
		auto [imageWidth, imageHeight, inFormat, outFormat, data] = loadTextureData(TEXTURE_ASSET_DIR + cubemapDirectory + "/" + std::to_string(i) + ".jpg");
		loadedCubemaps.at(cubemapDirectory)->uploadData(imageWidth, imageHeight, data, i);
		stbi_image_free(data);
	}
	return loadedCubemaps.at(cubemapDirectory);
}

void TextureLoader::unloadAll() {
	loadedTextures.clear();
	loadedCubemaps.clear();
}
