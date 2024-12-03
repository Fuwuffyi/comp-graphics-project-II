#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUv;

out vec4 col;

uniform mat4 objMatrix;
uniform mat4 cameraMatrix;

void main() {
    gl_Position = objMatrix * cameraMatrix * vec4(aPos, 1.0);
    col = vec4(aNormal, 1.0);
}