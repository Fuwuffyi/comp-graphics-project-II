#version 330 core

#define MAX_LIGHTS 32u

out vec4 fragColor;

in vec3 normalIn;
in vec2 uvIn;
in vec3 worldPosition;

uniform vec3 cameraPosition;

uniform vec4 material_color;
uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;

struct Light {
	vec3 position;
	uint type;
	vec3 direction;
	float range;
	vec3 ambient;
	float constant;
	vec3 diffuse;
	float linear;
	vec3 specular;
	float quadratic;
	float cutOff;
	float outerCutOff;
	vec2 padding;
};

layout(std140) uniform lightsBuffer{
	Light[MAX_LIGHTS] lights;
};

vec3 directionalLight(Light light, vec3 normal, vec3 viewDir);
vec3 pointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 spotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 calcDiffuse(vec3 lightDiffuse, float diffuseFactor);
vec3 calcAmbient(vec3 lightAmbient);
vec3 calcSpecular(vec3 lightSpecular, float specularFactor);

bool isTextureValid(sampler2D tex);

void main() {
	vec3 combinedLighting = vec3(0.0);
	vec3 viewDir = normalize(cameraPosition - worldPosition);
	vec3 normal = normalIn;
	if (isTextureValid(normal0)) {
		normal = texture(normal0, uvIn).xyz;
	}
	for (uint i = 0u; i < MAX_LIGHTS; ++i) {
		Light light = lights[i];
		if (light.type == 0u) {
			continue;
		} else if (light.type == 1u) {
			combinedLighting += directionalLight(light, normal, viewDir);
		} else if (light.type == 2u) {
			combinedLighting += pointLight(light, normal, worldPosition, viewDir);
		} else if (light.type == 3u) {
			combinedLighting += spotLight(light, normal, worldPosition, viewDir);
		}
	}
	if (isTextureValid(diffuse0)) {
		fragColor = vec4(texture(diffuse0, uvIn).xyz * combinedLighting, 1.0);
	} else {
		fragColor = vec4(material_diffuse.xyz * combinedLighting, 1.0);
	}
}

bool isTextureValid(sampler2D tex) {
	return texture(tex, vec2(0.5, 0.5)) != vec4(0.0, 0.0, 0.0, 0.0);
}

vec3 calcDiffuse(vec3 lightDiffuse, float diffuseFactor) {
	if (isTextureValid(diffuse0)) {
		return texture(diffuse0, uvIn).xyz * lightDiffuse * diffuseFactor;
	}
	return material_diffuse.xyz * lightDiffuse * diffuseFactor;
}

vec3 calcSpecular(vec3 lightSpecular, float specularFactor) {
	if (isTextureValid(specular0)) {
		return texture(specular0, uvIn).xyz * lightSpecular * specularFactor;
	}
	return material_specular.xyz * lightSpecular * specularFactor;
}

vec3 directionalLight(Light light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// Ambient
	vec3 ambient = material_ambient.xyz * light.ambient;
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = calcDiffuse(light.diffuse, diff);
	// Specular
	vec3 halfVec = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfVec), 0.0), material_shininess);
	vec3 specular = calcSpecular(light.specular, spec);
	// Return values
	return (ambient + diffuse + specular);
}

vec3 pointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float distance = length(light.position - fragPos);
	// Check if the fragment is outside the light's range
	if (distance > light.range) {
		return vec3(0.0);
	}
	// Attenuation based on distance
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// Clamp attenuation to avoid exceeding range
	attenuation = max(attenuation, 0.0);
	// Ambient
	vec3 ambient = material_ambient.xyz * light.ambient;
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = calcDiffuse(light.diffuse, diff);
	// Specular
	vec3 halfVec = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfVec), 0.0), material_shininess);
	vec3 specular = calcSpecular(light.specular, spec);
	// Return values
	return attenuation * (ambient + diffuse + specular);
}

vec3 spotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float distance = length(light.position - fragPos);
	// Check if the fragment is outside the light's range
	if (distance > light.range) {
		return vec3(0.0);
	}
	// Attenuation based on distance
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	attenuation = max(attenuation, 0.0);
	// Spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// Ambient
	vec3 ambient = material_ambient.xyz * light.ambient;
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = calcDiffuse(light.diffuse, diff);
	// Specular
	vec3 halfVec = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfVec), 0.0), material_shininess);
	vec3 specular = calcSpecular(light.specular, spec);
	// Return values
	return attenuation * intensity * (ambient + diffuse + specular);
}