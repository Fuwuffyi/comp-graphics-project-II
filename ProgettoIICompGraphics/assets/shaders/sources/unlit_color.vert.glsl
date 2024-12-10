#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUv;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 objMatrix;
uniform mat4 cameraMatrix;

void main() {
    vec3 worldPosition = vec3(objMatrix * vec4(aPos, 1.0));
    gl_Position = cameraMatrix * vec4(worldPosition, 1.0);
}