#version 330 core

#define MAX_LIGHTS 32u

out vec4 fragColor;

in vec3 normalIn;
in vec3 worldPosition;

uniform vec3 cameraPosition;

uniform vec4 material_color;
uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;

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

vec4 directionalLight(Light light, vec3 normal, vec3 viewDir);
vec4 pointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 spotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	vec4 combinedLighting = vec4(0.0);
	vec3 viewDir = normalize(cameraPosition - worldPosition);
	for (uint i = 0u; i < MAX_LIGHTS; ++i) {
		Light light = lights[i];
		if (light.type == 0u) {
			continue;
		}
		else if (light.type == 1u) {
			combinedLighting += directionalLight(light, normalIn, viewDir);
		}
		else if (light.type == 2u) {
			combinedLighting += pointLight(light, normalIn, worldPosition, viewDir);
		}
		else if (light.type == 3u) {
			combinedLighting += spotLight(light, normalIn, worldPosition, viewDir);
		}
	}
	fragColor = material_color * combinedLighting;
}

vec4 directionalLight(Light light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// Ambient
	vec4 ambient = material_ambient * vec4(light.ambient, 1.0);
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = material_diffuse * vec4(light.diffuse, 1.0) * diff;
	// Specular
	vec3 halfVec = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfVec), 0.0), material_shininess);
	vec4 specular = material_specular * vec4(light.specular, 1.0) * spec;
	// Return values
	return (ambient + diffuse + specular);
}

vec4 pointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float distance = length(light.position - fragPos);
	// Check if the fragment is outside the light's range
	if (distance > light.range) {
		return vec4(vec3(0.0), material_color.a);
	}
	// Attenuation based on distance
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// Clamp attenuation to avoid exceeding range
	attenuation = max(attenuation, 0.0);
	// Ambient
	vec4 ambient = material_ambient * vec4(light.ambient, 1.0);
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = material_diffuse * vec4(light.diffuse, 1.0) * diff;
	// Specular
	vec3 halfVec = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfVec), 0.0), material_shininess);
	vec4 specular = material_specular * vec4(light.specular, 1.0) * spec;
	// Return values
	return attenuation * (ambient + diffuse + specular);
}

vec4 spotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float distance = length(light.position - fragPos);
	// Check if the fragment is outside the light's range
	if (distance > light.range) {
		return vec4(vec3(0.0), material_color.a);
	}
	// Attenuation based on distance
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	attenuation = max(attenuation, 0.0);
	// Spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// Ambient
	vec4 ambient = material_ambient * vec4(light.ambient, 1.0);
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = material_diffuse * vec4(light.diffuse, 1.0) * diff;
	// Specular
	vec3 halfVec = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfVec), 0.0), material_shininess);
	vec4 specular = material_specular * vec4(light.specular, 1.0) * spec;
	// Return values
	return attenuation * intensity * (ambient + diffuse + specular);
}