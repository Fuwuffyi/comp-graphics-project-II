#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUv;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 normalIn;
out vec2 uvIn;
out vec3 worldPosition;
out mat3 TBN;

uniform float glfwTime;
uniform mat4 objMatrix;
uniform mat4 cameraMatrix;
uniform vec3 material_windDirection;
uniform float material_windStrength;

void main() {
    worldPosition = vec3(objMatrix * vec4(aPos, 1.0));
    worldPosition += material_windDirection * sin(glfwTime + 0.5 * length(worldPosition * material_windDirection)) * material_windStrength;
    gl_Position = cameraMatrix * vec4(worldPosition, 1.0);
    mat3 normalMatrix = transpose(inverse(mat3(objMatrix)));
    normalIn = normalize(normalMatrix * aNormal);
    uvIn = aUv;
    vec3 tangent = normalize(mat3(objMatrix) * aTangent);
    vec3 bitangent = normalize(mat3(objMatrix) * aBitangent);
    TBN = mat3(tangent, bitangent, normalIn);
}