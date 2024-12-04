#include "Material.hpp"

#include "Shader.hpp"

#include <stdexcept>

Material::Material(const std::string& _name, Shader* _shader, const std::unordered_map<std::string, Material::MaterialValueType>& values, const bool _lit, const bool _transparent)
	:
	shader(_shader),
	materialValues(values),
	lit(_lit),
	trasnparent(_transparent),
	name(_name)
{
	if (this->shader == nullptr) {
		std::runtime_error("The material has been initialized without a shader!");
	}
}

Material::~Material() {
	this->shader = nullptr;
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

bool Material::isTransparent() const {
	return this->trasnparent;
}

bool Material::isLit() const {
	return this->lit;
}
