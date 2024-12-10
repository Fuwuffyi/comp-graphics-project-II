#version 330 core

#define MAX_LIGHTS 32u

out vec4 fragColor;

in vec3 normalIn;
in vec2 uvIn;
in vec3 worldPosition;
in mat3 normalMatrix;
in mat3 TBN;

uniform vec3 cameraPosition;

uniform vec4 material_color;
uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;
uniform float material_cutoutThreshold;

uniform sampler2D albedo0;
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

vec4 directionalLight(Light light, vec3 normal, vec3 viewDir);
vec4 pointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 spotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec4 calcDiffuse(vec3 lightDiffuse, float diffuseFactor);
vec4 calcAmbient(vec3 lightAmbient);
vec4 calcSpecular(vec3 lightSpecular, float specularFactor);

bool isTextureValid(sampler2D tex);

void main() {
	vec4 combinedLighting = vec4(0.0);
	vec3 viewDir = normalize(cameraPosition - worldPosition);
	vec3 normal = normalIn;
	if (isTextureValid(normal0)) {
		normal = normalize(TBN * texture(normal0, uvIn).xyz);
	}
	for (uint i = 0u; i < MAX_LIGHTS; ++i) {
		Light light = lights[i];
		if (light.type == 0u) {
			continue;
		}
		else if (light.type == 1u) {
			combinedLighting += directionalLight(light, normal, viewDir);
		}
		else if (light.type == 2u) {
			combinedLighting += pointLight(light, normal, worldPosition, viewDir);
		}
		else if (light.type == 3u) {
			combinedLighting += spotLight(light, normal, worldPosition, viewDir);
		}
	}
	if (isTextureValid(albedo0)) {
		vec4 matColor = texture(albedo0, uvIn);
		if (matColor.a <= material_cutoutThreshold) {
			discard;
		}
		fragColor = matColor * combinedLighting;
	} else {
		if (material_color.a <= material_cutoutThreshold) {
			discard;
		}
		fragColor = material_color * combinedLighting;
	}
}

bool isTextureValid(sampler2D tex) {
	return texture(tex, vec2(0.5, 0.5)) != vec4(0.0, 0.0, 0.0, 0.0);
}

vec4 calcDiffuse(vec3 lightDiffuse, float diffuseFactor) {
	if (isTextureValid(diffuse0)) {
		return texture(diffuse0, uvIn) * vec4(lightDiffuse, 1.0) * diffuseFactor;
	}
	return material_diffuse * vec4(lightDiffuse, 1.0) * diffuseFactor;
}

vec4 calcSpecular(vec3 lightSpecular, float specularFactor) {
	if (isTextureValid(specular0)) {
		return texture(specular0, uvIn) * vec4(lightSpecular, 1.0) * specularFactor;
	}
	return material_specular * vec4(lightSpecular, 1.0) * specularFactor;
}

vec4 directionalLight(Light light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// Ambient
	vec4 ambient = material_ambient * vec4(light.ambient, 1.0);
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = calcDiffuse(light.diffuse, diff);
	// Specular
	vec4 specular = vec4(0.0);
	if (material_shininess != 0.0) {
		vec3 halfVec = normalize(viewDir + lightDir);
		float spec = pow(max(dot(normal, halfVec), 0.0), material_shininess);
		specular = calcSpecular(light.specular, spec);
	}
	// Return values
	return (ambient + diffuse + specular);
}

vec4 pointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float distance = length(light.position - fragPos);
	// Check if the fragment is outside the light's range
	if (distance > light.range) {
		return vec4(0.0);
	}
	// Attenuation based on distance
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// Clamp attenuation to avoid exceeding range
	attenuation = max(attenuation, 0.0);
	// Ambient
	vec4 ambient = material_ambient * vec4(light.ambient, 1.0);
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = calcDiffuse(light.diffuse, diff);
	// Specular
	vec4 specular = vec4(0.0);
	if (material_shininess != 0.0) {
		vec3 halfVec = normalize(viewDir + lightDir);
		float spec = pow(max(dot(normal, halfVec), 0.0), material_shininess);
		specular = calcSpecular(light.specular, spec);
	}
	// Return values
	return attenuation * (ambient + diffuse + specular);
}

vec4 spotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float distance = length(light.position - fragPos);
	// Check if the fragment is outside the light's range
	if (distance > light.range) {
		return vec4(0.0);
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
	vec4 diffuse = calcDiffuse(light.diffuse, diff);
	// Specular
	vec4 specular = vec4(0.0);
	if (material_shininess != 0.0) {
		vec3 halfVec = normalize(viewDir + lightDir);
		float spec = pow(max(dot(normal, halfVec), 0.0), material_shininess);
		specular = calcSpecular(light.specular, spec);
	}
	// Return values
	return attenuation * intensity * (ambient + diffuse + specular);
}