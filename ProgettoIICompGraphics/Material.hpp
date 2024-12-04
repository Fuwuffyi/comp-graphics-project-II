#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <variant>
#include <string>

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
	Shader* shader;
	std::unordered_map<std::string, MaterialValueType> materialValues;
	bool trasnparent;
	bool lit;

public:
	const std::string name;

	/**
	 * Constructor for a material.
	 * 
	 * \param _name The material's name.
	 * \param _shader A shader pointer to be used to render the material.
	 * \param values The properties of the material.
	 * \param _lit Checks if a material will be affected by lighting.
	 * \param _transparent Checks if a material will be transparent.
	 */
	Material(const std::string& _name, Shader* _shader, const std::unordered_map<std::string, MaterialValueType>& values, const bool _lit = false, const bool _transparent = false);

	/**
	 * Destructor for the material class.
	 * 
	 */
	~Material();

	/**
	 * Checks if the material can be transparent.
	 *
	 * \return True if it is transparent.
	 */
	bool isTransparent() const;

	/**
	 * Checks if the material is affected by lighting.
	 * 
	 * \return True if it is affected by lights.
	 */
	bool isLit() const;

	/**
	 * Activates the material's shader and its properties.
	 * 
	 */
	void activate() const;
};
 