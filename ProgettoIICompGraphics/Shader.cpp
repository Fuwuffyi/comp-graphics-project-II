#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <iostream>

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

Shader::Shader(const std::string& _name, const std::string& vertexSource, const std::string& fragmentSource, const bool _litFlag, const bool _transparentFlag)
	:
	uniformLocations(),
	name(_name),
	id(glCreateProgram()),
	litFlag(_litFlag),
	transparentFlag(_transparentFlag)
{
	const char* vertSource = vertexSource.c_str();
	const char* fragSource = fragmentSource.c_str();
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
	char* uniformName = new char[static_cast<size_t>(maxUniformNameLength)];
	for (uint32_t i = 0; i < static_cast<uint32_t>(numUniforms); ++i) {
		int32_t size;
		uint32_t type;
		glGetActiveUniform(this->id, i, maxUniformNameLength, nullptr, &size, &type, uniformName);
		const int32_t loc = glGetUniformLocation(this->id, uniformName);
		this->uniformLocations[uniformName] = loc;
	}
	delete[] uniformName;
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

const std::string& Shader::getName() const {
	return this->name;
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
