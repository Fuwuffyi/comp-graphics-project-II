#include "Material.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

#include <stdexcept>

Material::Material(const std::shared_ptr<Shader>& _shader, const std::unordered_map<std::string, MaterialValueType>& _values, const std::unordered_map<std::string, std::shared_ptr<Texture>>& _textures)
	:
	shader(_shader),
	values(_values)
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

std::unordered_map<std::string, Material::MaterialValueType>& Material::getMutableProperties() {
	return this->values;
}

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
			this->shader->setUniform(uniform, val);
		};
		std::visit(visitor, value);
	}
	// Setup all shader uniform properties
	for (const auto& [uniform, texturePtr] : this->textures) {
		static int32_t bindingPoint = 0;
		texturePtr->activate(bindingPoint);
		this->shader->setUniform(uniform, bindingPoint++);
	}
}