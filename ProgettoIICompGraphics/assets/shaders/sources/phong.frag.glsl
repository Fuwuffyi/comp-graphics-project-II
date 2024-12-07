#version 330 core

out vec4 fragColor;

in vec3 normalIn;
in vec3 worldPosition;

uniform vec4 materialColor;
uniform vec3 cameraPosition;

// Eventually to use SSBO or uniforms
vec3 lightColor = vec3(1.0);
vec3 lightPos = vec3(0.0, 0.25, 2.0);
float ambient = 0.2;
float specular = 0.5;

void main() {
    vec3 normal = normalize(normalIn);
    vec3 lightDir = normalize(lightPos - worldPosition);
    float diffuse = max(dot(normal, lightDir), 0.0);

    vec3 viewDir = normalize(cameraPosition - worldPosition);
    vec3 reflectionDir = reflect(-lightDir, normal);
    float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0), 8.0);
    float specularValue = specAmount * specular;

    fragColor = vec4(materialColor.xyz * (lightColor * (ambient + diffuse + specularValue)), 1.0);
}