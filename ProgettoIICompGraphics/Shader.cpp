#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <stdexcept>
#include <fstream>
#include <memory>
#include <iostream>

std::string Shader::readShaderSource(const std::string& shaderFile) {
	std::ifstream file(shaderFile, std::ios::binary);
	if (!file) {
		throw std::runtime_error("Failed to open shader file");
	}
	file.seekg(0, std::ios::end);
	const std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::string contents(static_cast<uint64_t>(size), ' ');
	file.read(&contents[0], static_cast<int64_t>(contents.size()));
	return contents;
}

void Shader::checkErrors(const std::string& shaderType, const uint32_t shaderId) const {
	// Check if shader has compiled successfully
	int32_t isCompiled;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled) {
		// Get error buffer length
		int32_t buffLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &buffLength);
		// Get error buffer
		std::vector<char> errorLog(buffLength);
		glGetShaderInfoLog(shaderId, buffLength, &buffLength, &errorLog[0]);
		// Print out error
		std::cerr << shaderType << " ERROR:" << ' ' << errorLog.data() << std::endl;
		// Delete shader if not compiled
		glDeleteShader(shaderId);
	}
}

Shader::Shader(const std::string& fragmentFile, const std::string& vertexFile)
	:
	id(glCreateProgram())
{
	// Load shader source code to c style strings
	const std::string vertCode = Shader::readShaderSource(vertexFile);
	const std::string fragCode = Shader::readShaderSource(fragmentFile);
	const char* vertSource = vertCode.c_str();
	const char* fragSource = fragCode.c_str();
	// Compile vertex shader
	const uint32_t vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSource, nullptr);
	glCompileShader(vertShader);
	this->checkErrors("VERTEX", vertShader);
	// Compile fragment shader
	const uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSource, nullptr);
	glCompileShader(fragShader);
	this->checkErrors("FRAGMENT", fragShader);
	// Create the program and link shaders
	glAttachShader(this->id, vertShader);
	glAttachShader(this->id, fragShader);
	glLinkProgram(this->id);
	// Free shader data
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	// Load all uniform names to map
	int32_t numUniforms;
	glGetProgramiv(this->id, GL_ACTIVE_UNIFORMS, &numUniforms);
	int32_t maxUniformNameLength;
	glGetProgramiv(this->id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
	// Create buffer and loop through all uniforms to save them
	const std::unique_ptr<GLchar[]> uniformName = std::make_unique<GLchar[]>(static_cast<uint64_t>(maxUniformNameLength));
	for (uint32_t i = 0; i < static_cast<uint32_t>(numUniforms); ++i) {
		int32_t size;
		GLenum type;
		glGetActiveUniform(this->id, i, maxUniformNameLength, nullptr, &size, &type, uniformName.get());
		const int32_t loc = glGetUniformLocation(this->id, uniformName.get());
		this->uniformLocations[uniformName.get()] = loc;
	}
}

Shader::~Shader() {
	glDeleteProgram(this->id);
}

void Shader::activate() const {
	glUseProgram(this->id);
}

int32_t Shader::getUniformLocation(const std::string& uniform) const {
	std::unordered_map<std::string, uint32_t>::const_iterator it = this->uniformLocations.find(uniform);
	if (it != this->uniformLocations.end()) {
		return it->second;
	}
	return -1;
}

void Shader::setUniform(const std::string& uniformName, const float v) const {
	glUniform1f(this->getUniformLocation(uniformName), v);
}

void Shader::setUniform(const std::string& uniformName, const float a, const float b) const {
	glUniform2f(this->getUniformLocation(uniformName), a, b);
}

void Shader::setUniform(const std::string& uniformName, const float a, const float b, const float c) const {
	glUniform3f(this->getUniformLocation(uniformName), a, b, c);
}

void Shader::setUniform(const std::string& uniformName, const float a, const float b, const float c, const float d) const {
	glUniform4f(this->getUniformLocation(uniformName), a, b, c, d);
}

void Shader::setUniform(const std::string& uniformName, const glm::vec2& v) const {
	glUniform2f(this->getUniformLocation(uniformName), v.x, v.y);
}

void Shader::setUniform(const std::string& uniformName, const glm::vec3& v) const {
	glUniform3f(this->getUniformLocation(uniformName), v.x, v.y, v.z);
}

void Shader::setUniform(const std::string& uniformName, const glm::vec4& v) const {
	glUniform4f(this->getUniformLocation(uniformName), v.x, v.y, v.z, v.w);
}

void Shader::setUniform(const std::string& uniformName, const int32_t v) const {
	glUniform1i(this->getUniformLocation(uniformName), v);
}

void Shader::setUniform(const std::string& uniformName, const int32_t a, const int32_t b) const {
	glUniform2i(this->getUniformLocation(uniformName), a, b);
}

void Shader::setUniform(const std::string& uniformName, const int32_t a, const int32_t b, const int32_t c) const {
	glUniform3i(this->getUniformLocation(uniformName), a, b, c);
}

void Shader::setUniform(const std::string& uniformName, const int32_t a, const int32_t b, const int32_t c, const int32_t d) const {
	glUniform4i(this->getUniformLocation(uniformName), a, b, c, d);
}

void Shader::setUniform(const std::string& uniformName, const glm::ivec2& v) const {
	glUniform2i(this->getUniformLocation(uniformName), v.x, v.y);
}

void Shader::setUniform(const std::string& uniformName, const glm::ivec3& v) const {
	glUniform3i(this->getUniformLocation(uniformName), v.x, v.y, v.z);
}

void Shader::setUniform(const std::string& uniformName, const glm::ivec4& v) const {
	glUniform4i(this->getUniformLocation(uniformName), v.x, v.y, v.z, v.w);
}

void Shader::setUniform(const std::string& uniformName, const uint32_t v) const {
	glUniform1ui(this->getUniformLocation(uniformName), v);
}

void Shader::setUniform(const std::string& uniformName, const uint32_t a, const uint32_t b) const {
	glUniform2ui(this->getUniformLocation(uniformName), a, b);
}

void Shader::setUniform(const std::string& uniformName, const uint32_t a, const uint32_t b, const uint32_t c) const {
	glUniform3ui(this->getUniformLocation(uniformName), a, b, c);
}

void Shader::setUniform(const std::string& uniformName, const uint32_t a, const uint32_t b, const uint32_t c, const uint32_t d) const {
	glUniform4ui(this->getUniformLocation(uniformName), a, b, c, d);
}

void Shader::setUniform(const std::string& uniformName, const glm::uvec2& v) const {
	glUniform2ui(this->getUniformLocation(uniformName), v.x, v.y);
}

void Shader::setUniform(const std::string& uniformName, const glm::uvec3& v) const {
	glUniform3ui(this->getUniformLocation(uniformName), v.x, v.y, v.z);
}

void Shader::setUniform(const std::string& uniformName, const glm::uvec4& v) const {
	glUniform4ui(this->getUniformLocation(uniformName), v.x, v.y, v.z, v.w);
}

void Shader::setUniformMatrix(const std::string& uniformName, const glm::mat2& v, const bool transpose) const {
	glUniformMatrix2fv(this->getUniformLocation(uniformName), 1, transpose, glm::value_ptr(v));
}

void Shader::setUniformMatrix(const std::string& uniformName, const glm::mat3& v, const bool transpose) const {
	glUniformMatrix3fv(this->getUniformLocation(uniformName), 1, transpose, glm::value_ptr(v));
}

void Shader::setUniformMatrix(const std::string& uniformName, const glm::mat4& v, const bool transpose) const {
	glUniformMatrix4fv(this->getUniformLocation(uniformName), 1, transpose, glm::value_ptr(v));
}
