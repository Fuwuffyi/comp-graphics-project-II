#version 330 core

#define MAX_LIGHTS 32u

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUv;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec4 lightingColor;
out vec2 uvIn;

uniform mat4 objMatrix;
uniform mat4 cameraMatrix;
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

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;

vec4 directionalLight(Light light, vec3 normal, vec3 viewDir);
vec4 pointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 spotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 worldPosition = vec3(objMatrix * vec4(aPos, 1.0));
    gl_Position = cameraMatrix * vec4(worldPosition, 1.0);
    uvIn = aUv;
    // Calculate normal
    mat3 normalMatrix = transpose(inverse(mat3(objMatrix)));
    vec3 normal = normalize(normalMatrix * aNormal);
    vec3 viewDir = normalize(cameraPosition - worldPosition);
    // Add lighting
    vec4 combinedLighting = vec4(0.0);
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
    // Pass color to fragment for interpolation
    lightingColor = combinedLighting;
}

vec4 directionalLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfVec = normalize(viewDir + lightDir);
    float spec = 0.0;
    if (material_shininess != 0.0) {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    }
    vec4 ambient = material_ambient * vec4(light.ambient, 1.0);
    vec4 diffuse = material_diffuse * vec4(light.diffuse, 1.0) * diff;
    vec4 specular = material_specular * vec4(light.specular, 1.0) * spec;
    return ambient + diffuse + specular;
}

vec4 pointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    if (distance > light.range) {
        return vec4(0.0);
    }
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    attenuation = max(attenuation, 0.0);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = 0.0;
    if (material_shininess != 0.0) {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    }
    vec4 ambient = material_ambient * vec4(light.ambient, 1.0);
    vec4 diffuse = material_diffuse * vec4(light.diffuse, 1.0) * diff;
    vec4 specular = material_specular * vec4(light.specular, 1.0) * spec;
    return attenuation * (ambient + diffuse + specular);
}

vec4 spotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    if (distance > light.range) {
        return vec4(0.0);
    }
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    attenuation = max(attenuation, 0.0);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = 0.0;
    if (material_shininess != 0.0) {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    }
    vec4 ambient = material_ambient * vec4(light.ambient, 1.0);
    vec4 diffuse = material_diffuse * vec4(light.diffuse, 1.0) * diff;
    vec4 specular = material_specular * vec4(light.specular, 1.0) * spec;
    return attenuation * intensity * (ambient + diffuse + specular);
}
