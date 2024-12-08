#version 330 core

#define MAX_LIGHTS 32u

out vec4 fragColor;

in vec3 normalIn;
in vec3 worldPosition;

uniform vec4 materialColor;
uniform vec3 cameraPosition;

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

void main() {
	vec3 combinedLighting = vec3(0.0);
	vec3 viewDir = normalize(cameraPosition - worldPosition);
	for (uint i = 0u; i < MAX_LIGHTS; ++i) {
		Light light = lights[i];
		if (light.type == 0u) {
			continue;
		} else if (light.type == 1u) {
			combinedLighting += directionalLight(light, normalIn, viewDir);
		} else if (light.type == 2u) {
			combinedLighting += pointLight(light, normalIn, worldPosition, viewDir);
		} else if (light.type == 3u) {
			combinedLighting += spotLight(light, normalIn, worldPosition, viewDir);
		}
	}
    fragColor = vec4(materialColor.xyz * combinedLighting, 1.0);
}

vec3 directionalLight(Light light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// Ambient
	vec3 ambient = light.ambient;
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff;
	// Specular
	vec3 halfVec = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfVec), 0.0), 8.0);
	vec3 specular = light.specular * spec;
	// Return values
	return (ambient + diffuse + specular);
}

vec3 pointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float distance = length(light.position - fragPos);
	// Check if the fragment is outside the light's range
	if (distance > light.range) return vec3(0.0);
	// Attenuation based on distance
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// Clamp attenuation to avoid exceeding range
	attenuation = max(attenuation, 0.0);
	// Ambient
	vec3 ambient = light.ambient;
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff;
	// Specular
	vec3 halfVec = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfVec), 0.0), 8.0);
	vec3 specular = light.specular * spec;
	// Return values
	return attenuation * (ambient + diffuse + specular);
}

vec3 spotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float distance = length(light.position - fragPos);
	// Check if the fragment is outside the light's range
	if (distance > light.range) return vec3(0.0);
	// Attenuation based on distance
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	attenuation = max(attenuation, 0.0);
	// Spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// Ambient
	vec3 ambient = light.ambient;
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff;
	// Specular
	vec3 halfVec = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfVec), 0.0), 8.0);
	vec3 specular = light.specular * spec;
	// Return values
	return attenuation * intensity * (ambient + diffuse + specular);
}