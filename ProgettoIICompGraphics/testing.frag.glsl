#version 330 core

out vec4 fragColor;

uniform vec4 testUniform;

void main() {
	fragColor = testUniform;
}