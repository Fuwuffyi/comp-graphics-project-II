#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUv;

out vec3 normalIn;
out vec2 uvIn;
out vec3 worldPosition;

uniform mat4 objMatrix;
uniform mat4 cameraMatrix;

void main() {
    worldPosition = vec3(objMatrix * vec4(aPos, 1.0));
    gl_Position = cameraMatrix * vec4(worldPosition, 1.0);
    mat3 normalMatrix = transpose(inverse(mat3(objMatrix)));
    normalIn = normalize(normalMatrix * aNormal);
    uvIn = aUv;
}