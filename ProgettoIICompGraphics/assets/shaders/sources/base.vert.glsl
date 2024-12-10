#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUv;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 normalIn;
out vec2 uvIn;
out vec3 worldPosition;
out mat3 normalMatrix;
out mat3 TBN;

uniform mat4 objMatrix;
uniform mat4 cameraMatrix;

void main() {
    worldPosition = vec3(objMatrix * vec4(aPos, 1.0));
    gl_Position = cameraMatrix * vec4(worldPosition, 1.0);
    normalMatrix = transpose(inverse(mat3(objMatrix)));
    normalIn = normalize(normalMatrix * aNormal);
    uvIn = aUv;

    vec3 tangent = normalize(mat3(objMatrix) * aTangent);
    vec3 bitangent = normalize(mat3(objMatrix) * aBitangent);
    TBN = mat3(tangent, bitangent, normalIn);
}