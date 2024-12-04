#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>
#include <glm/glm.hpp>

/**
 * Class holding information of a shader.
 */
class Shader {
private:
	/**
	 * Checks and prints error of given shader sources.
	 *
	 * \param shaderType The type of the shader to log.
	 * \param shaderId The id of the shader to check errors for.
	 */
	void checkErrors(const std::string& shaderType, const uint32_t shaderId) const;

	std::unordered_map<std::string, uint32_t> uniformLocations; // Contains all of the uniform variable locations

	uint32_t id;
public:
	// Erase copy constructors, as it would break opengl
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;


	/**
	 * Creates a new shader program from a fragment and vertex shader.
	 *
	 * \param vertexSource The vertex shader's code.
	 * \param fragmentSile The fragment shader's code.
	 */
	Shader(const std::string& vertexSource, const std::string& fragmentSile);

	/**
	 * Deallocates the GPU memory for this shader program.
	 *
	 */
	~Shader();

	/**
	 * Activates the shader to use for all next rendered objects.
	 *
	 */
	void activate() const;

	/**
	 * Getter for an uniform location within the shader.
	 *
	 * \param uniform The uniform to get the location of.
	 * \return The uniform's location within the shader, -1 if it does not exist.
	 */
	int32_t getUniformLocation(const std::string& uniform) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * One float version.
	 * 
	 * \param uniformName The uniform location's name
	 * \param v The value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const float v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Two float version.
	 *
	 * \param uniformName The uniform location's name
	 * \param a The first value to set on the shader.
	 * \param b The second value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const float a, const float b) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Three float version.
	 *
	 * \param uniformName The uniform location's name
	 * \param a The first value to set on the shader.
	 * \param b The second value to set on the shader.
	 * \param c The second value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const float a, const float b, const float c) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Four float version.
	 *
	 * \param uniformName The uniform location's name
	 * \param a The first value to set on the shader.
	 * \param b The second value to set on the shader.
	 * \param c The second value to set on the shader.
	 * \param d The second value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const float a, const float b, const float c, const float d) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Vec2 version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The values to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const glm::vec2& v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Vec3 version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The values to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const glm::vec3& v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Vec4 version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The values to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const glm::vec4& v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * One integer version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const int32_t v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Two integer version.
	 *
	 * \param uniformName The uniform location's name
	 * \param a The first value to set on the shader.
	 * \param b The second value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const int32_t a, const int32_t b) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Three integer version.
	 *
	 * \param uniformName The uniform location's name
	 * \param a The first value to set on the shader.
	 * \param b The second value to set on the shader.
	 * \param c The second value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const int32_t a, const int32_t b, const int32_t c) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Four integer version.
	 *
	 * \param uniformName The uniform location's name
	 * \param a The first value to set on the shader.
	 * \param b The second value to set on the shader.
	 * \param c The second value to set on the shader.
	 * \param d The second value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const int32_t a, const int32_t b, const int32_t c, const int32_t d) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Int vec2 version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The values to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const glm::ivec2& v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Int vec3 version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The values to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const glm::ivec3& v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Int vec4 version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The values to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const glm::ivec4& v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * One unsigned integer version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const uint32_t v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Two unsigned integer version.
	 *
	 * \param uniformName The uniform location's name
	 * \param a The first value to set on the shader.
	 * \param b The second value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const uint32_t a, const uint32_t b) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Three unsigned integer version.
	 *
	 * \param uniformName The uniform location's name
	 * \param a The first value to set on the shader.
	 * \param b The second value to set on the shader.
	 * \param c The second value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const uint32_t a, const uint32_t b, const uint32_t c) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Four unsigned integer version.
	 *
	 * \param uniformName The uniform location's name
	 * \param a The first value to set on the shader.
	 * \param b The second value to set on the shader.
	 * \param c The second value to set on the shader.
	 * \param d The second value to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const uint32_t a, const uint32_t b, const uint32_t c, const uint32_t d) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Unsigned int vec2 version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The values to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const glm::uvec2& v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Unsigned int vec3 version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The values to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const glm::uvec3& v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * Unsigned int vec4 version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The values to set on the shader.
	 */
	void setUniform(const std::string& uniformName, const glm::uvec4& v) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * 2x2 matrix version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The matrix to set on the shader.
	 * \param transpose Flag to set if it should transpose the matrix.
	 */
	void setUniformMatrix(const std::string& uniformName, const glm::mat2& v, const bool transpose = false) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * 3x3 matrix version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The matrix to set on the shader.
	 * \param transpose Flag to set if it should transpose the matrix.
	 */
	void setUniformMatrix(const std::string& uniformName, const glm::mat3& v, const bool transpose = false) const;

	/**
	 * Sets a uniform on the shader (provided the shader is active, and the uniform exists).
	 * 4x4 matrix version.
	 *
	 * \param uniformName The uniform location's name
	 * \param v The matrix to set on the shader.
	 * \param transpose Flag to set if it should transpose the matrix.
	 */
	void setUniformMatrix(const std::string& uniformName, const glm::mat4& v, const bool transpose = false) const;
};