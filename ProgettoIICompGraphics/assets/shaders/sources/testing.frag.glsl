#version 330 core

out vec4 fragColor;

uniform vec4 materialColor;

in vec3 normal;

void main() {
	fragColor = materialColor * vec4(normal, 1.0);
}