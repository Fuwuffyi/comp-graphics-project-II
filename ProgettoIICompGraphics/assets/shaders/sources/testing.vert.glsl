#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUv;

out vec3 normal;

uniform mat4 objMatrix;
uniform mat4 cameraMatrix;

void main() {
    gl_Position = cameraMatrix * objMatrix * vec4(aPos, 1.0);
    mat3 normalMatrix = transpose(inverse(mat3(objMatrix)));
    normal = normalize(normalMatrix * aNormal);
}