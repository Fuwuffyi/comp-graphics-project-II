#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <variant>
#include <string>
#include <memory>

class Texture;

/**
 * Forward declaration for the shader class.
 */
class Shader;

class Material {
public:
	/**
	 * Types allowed for material properties.
	 */
	using MaterialValueType = std::variant<
		float, glm::vec2, glm::vec3, glm::vec4, 
		int32_t, glm::ivec2, glm::ivec3, glm::ivec4, 
		uint32_t, glm::uvec2, glm::uvec3, glm::uvec4
	>;

private:
	std::shared_ptr<Shader> shader;
	std::unordered_map<std::string, MaterialValueType> values;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

public:
	const std::string name;

	/**
	 * Constructor for a material.
	 * 
	 * \param _name The material's name.
	 * \param _shader A shader pointer to be used to render the material.
	 * \param values The properties of the material.
	 * \param textures The material's loaded textures.
	 */
	Material(const std::string& _name, const std::shared_ptr<Shader>& _shader, const std::unordered_map<std::string, MaterialValueType>& _values, const std::unordered_map<std::string, std::shared_ptr<Texture>>& _textures);

	/**
	 * Destructor for the material class.
	 * 
	 */
	~Material();

	/*
	* Gets tue shader pointer.
	* 
	* \return The material's shader pointer.
	*/
	Shader* getShader() const;

	/**
	 * Activates the material's shader and its properties.
	 * 
	 */
	void activate() const;
};
 