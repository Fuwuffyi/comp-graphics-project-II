#version 330 core

out vec4 fragColor;

in vec3 uv;

uniform vec4 testing;

void main() {
    fragColor = vec4(uv, 1.0);
}