#include "LightSystem.hpp"

#include "UniformBuffer.hpp"

namespace LightSystem {
    static Lights lights;
    static UniformBuffer* lightsBuffer;
}

void LightSystem::initialize() {
	lightsBuffer = new UniformBuffer();
    lightsBuffer->bind();
    lightsBuffer->uploadData(&lights, sizeof(Lights));
}

void LightSystem::setLight(const size_t position, const DirectionalLight& directionalLight) {
	lightsBuffer->bind();
	Light light{};
	light.type = LIGHT_TYPE::DIRECTIONAL;
	light.direction = glm::normalize(directionalLight.direction);
	light.ambient = directionalLight.ambient;
	light.diffuse = directionalLight.diffuse;
	light.specular = directionalLight.specular;
	lights.lights[position] = light;
	lightsBuffer->uploadSubData(&light, sizeof(Light), position * sizeof(Light));
}

void LightSystem::setLight(const size_t position, const PointLight& pointLight) {
	lightsBuffer->bind();
	Light light{};
	light.type = LIGHT_TYPE::POINT;
	light.position = pointLight.position;
	light.ambient = pointLight.ambient;
	light.diffuse = pointLight.diffuse;
	light.specular = pointLight.specular;
	light.range = pointLight.range;
	light.constant = pointLight.constant;
	light.linear = pointLight.linear;
	light.quadratic = pointLight.quadratic;
	lights.lights[position] = light;
	lightsBuffer->uploadSubData(&light, sizeof(Light), position * sizeof(Light));
}

void LightSystem::setLight(const size_t position, const SpotLight& spotLight) {
	lightsBuffer->bind();
	Light light{};
	light.type = LIGHT_TYPE::SPOT;
	light.position = spotLight.position;
	light.direction = glm::normalize(spotLight.direction);
	light.ambient = spotLight.ambient;
	light.diffuse = spotLight.diffuse;
	light.specular = spotLight.specular;
	light.range = spotLight.range;
	light.constant = spotLight.constant;
	light.linear = spotLight.linear;
	light.quadratic = spotLight.quadratic;
	light.cutOff = spotLight.cutOff;
	light.outerCutOff = spotLight.outerCutOff;
	lights.lights[position] = light;
	lightsBuffer->uploadSubData(&light, sizeof(Light), position * sizeof(Light));
}

void LightSystem::setLight(const size_t position, const Light& anyLight) {
	lights.lights[position] = anyLight;
	lightsBuffer->uploadSubData(&anyLight, sizeof(Light), position * sizeof(Light));
}

void LightSystem::eraseLight(const size_t position) {
	Light light{};
	light.type = LIGHT_TYPE::NONE;
	lights.lights[position] = light;
	lightsBuffer->uploadSubData(&light, sizeof(Light), position * sizeof(Light));
}

void LightSystem::enableAt(const uint32_t bindingPoint) {
	lightsBuffer->bind();
	lightsBuffer->activate(bindingPoint);
}

LightSystem::Lights& LightSystem::getAllLights() {
	return lights;
}
