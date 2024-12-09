#include "Material.hpp"

#include "Shader.hpp"
#include "Texture.hpp"
#include <stdexcept>

Material::Material(const std::string& _name, const std::shared_ptr<Shader>& _shader, const std::unordered_map<std::string, MaterialValueType>& _values, const std::unordered_map<std::string, std::shared_ptr<Texture>>& _textures)
	:
	shader(_shader),
	values(_values),
	textures(_textures),
	name(_name)
{
	if (this->shader == nullptr) {
		std::runtime_error("The material has been initialized without a shader!");
	}
}

Material::~Material() {
	this->shader = nullptr;
}

Shader* Material::getShader() const {
	return this->shader.get();
}

void Material::setShader(const std::shared_ptr<Shader>& _shader) {
	this->shader = _shader;
}

std::unordered_map<std::string, Material::MaterialValueType>& Material::getMutableProperties() {
	return this->values;
}

#include <iostream>
void Material::activate() const {
	if (!this->shader) {
		return;
	}
	// Activate the shader
	this->shader->activate();
	// Setup all shader uniform properties
	for (const auto& [uniform, value] : this->values) {
		const auto visitor = [&uniform, this](const auto& val) {
			using T = std::decay_t<decltype(val)>;
			this->shader->setUniform("material_" + uniform, val);
		};
		std::visit(visitor, value);
	}
	this->shader->setUniform("useTexture", this->textures.size() > 0 ? 1 : 0);
	// Setup all shader uniform properties
	for (const auto& [uniform, texturePtr] : this->textures) {
		static int32_t bindingPoint = 0;
		std::cout << bindingPoint << std::endl;
		texturePtr->activate(bindingPoint);
		this->shader->setUniform(uniform, bindingPoint++);
	}
}