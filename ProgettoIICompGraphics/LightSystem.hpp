#pragma once

#include <glm/glm.hpp>

namespace LightSystem {
	// Same value on shader
	static constexpr size_t MAX_LIGHTS = 32;

	enum class LIGHT_TYPE : uint32_t {
		NONE = 0,
		DIRECTIONAL = 1,
		POINT = 2,
		SPOT = 3
	};

	struct DirectionalLight {
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct PointLight {
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float range;
		float constant;
		float linear;
		float quadratic;
	};

	struct SpotLight {
		glm::vec3 position;
		glm::vec3 direction;
		float cutOff;
		float outerCutOff;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float range;
		float constant;
		float linear;
		float quadratic;
	};

	// Arranged this way to assure padding = 4N for the shader to work
	struct Light {
		glm::vec3 position;
		LIGHT_TYPE type;
		glm::vec3 direction;
		float range;
		glm::vec3 ambient;
		float constant;
		glm::vec3 diffuse;
		float linear;
		glm::vec3 specular;
		float quadratic;
		float cutOff;
		float outerCutOff;
		glm::vec2 pad = glm::vec2(0.0f);
	};

	struct Lights {
		Light lights[MAX_LIGHTS];
	};

	void initialize();
	void setLight(const size_t position, const DirectionalLight& directionalLight);
	void setLight(const size_t position, const PointLight& pointLight);
	void setLight(const size_t position, const SpotLight& spotLight);
	void eraseLight(const size_t position);
	void enableAt(const uint32_t bindingPoint);
}
