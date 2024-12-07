#include "Material.hpp"

#include "Shader.hpp"

#include <stdexcept>

Material::Material(const std::string& _name, const std::shared_ptr<Shader>& _shader, const std::unordered_map<std::string, MaterialValueType>& values)
	:
	shader(_shader),
	materialValues(values),
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

void Material::activate() const {
	if (!this->shader) {
		return;
	}
	this->shader->activate();
	for (const std::pair<std::string, Material::MaterialValueType>& keyValuePair : this->materialValues) {
		const std::string& uniformName = keyValuePair.first;
		const MaterialValueType& value = keyValuePair.second;
		const auto visitor = [&uniformName, this](const auto& val) {
			using T = std::decay_t<decltype(val)>;
			this->shader->setUniform(uniformName, val);
		};
		std::visit(visitor, value);
	}
}