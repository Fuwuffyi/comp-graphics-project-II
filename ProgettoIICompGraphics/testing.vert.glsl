#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aNormal;
layout(location = 2) in vec2 aUv;

uniform mat4 objMatrix;

void main() {
    gl_Position = objMatrix * vec4(aPos, 0.0, 1.0);
}