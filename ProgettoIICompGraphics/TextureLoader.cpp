#include "TextureLoader.hpp"

#include "Texture.hpp"
#include "Texture2D.hpp"
#include "TextureCubemap.hpp"

#include <stb_image.h>
#include <unordered_map>
#include <stdexcept>

namespace TextureLoader {
	static std::unordered_map<std::string, std::pair<uint32_t, Texture2D>> loadedTextures;
	static std::unordered_map<std::string, std::pair<uint32_t, TextureCubemap>> loadedCubemaps;

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

Texture* TextureLoader::load(const std::string& textureName, const uint32_t binding, const bool flipImage) {
	// Return null pointer if empty
	if (textureName.empty()) {
		return nullptr;
	}
	// If shader already loaded, return ref
	if (loadedTextures.find(textureName) != loadedTextures.end()) {
		++loadedTextures.at(textureName).first;
		return &loadedTextures.at(textureName).second;
	}
	auto [imageWidth, imageHeight, inFormat, outFormat, data] = loadTextureData(TEXTURE_ASSET_DIR + textureName);
	loadedTextures.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(textureName),
		std::forward_as_tuple(
			std::piecewise_construct,
			std::forward_as_tuple(0u),
			std::forward_as_tuple(binding, inFormat, outFormat)
		)
	);
	loadedTextures.at(textureName).second.uploadData(imageWidth, imageHeight, data);
	stbi_image_free(data);
	return &loadedTextures.at(textureName).second;
}

Texture* TextureLoader::loadCubemap(const std::string& cubemapDirectory, const uint32_t binding) {
	// Return null pointer if empty
	if (cubemapDirectory.empty()) {
		return nullptr;
	}
	// If shader already loaded, return ref
	if (loadedTextures.find(cubemapDirectory) != loadedTextures.end()) {
		++loadedTextures.at(cubemapDirectory).first;
		return &loadedTextures.at(cubemapDirectory).second;
	}
	loadedCubemaps.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(cubemapDirectory),
		std::forward_as_tuple(
			std::piecewise_construct,
			std::forward_as_tuple(0u),
			std::forward_as_tuple(binding)
		)
	);
	// Load faces of cubemap
	for (uint32_t i = 0; i < 6; ++i) {
		auto [imageWidth, imageHeight, inFormat, outFormat, data] = loadTextureData(TEXTURE_ASSET_DIR + cubemapDirectory + "/" + std::to_string(i) + ".jpg");
		loadedCubemaps.at(cubemapDirectory).second.uploadData(imageWidth, imageHeight, data, i);
		stbi_image_free(data);
	}
	return &loadedCubemaps.at(cubemapDirectory).second;
}

void TextureLoader::unload(const std::string& textureName) {
	auto it = loadedTextures.find(textureName);
	if (it == loadedTextures.end()) {
		return;
	}
	if (--it->second.first == 0u) {
		loadedTextures.erase(textureName);
	}
}

void TextureLoader::unloadAll() {
	loadedTextures.clear();
}
