#version 330 core

out vec4 fragColor;

in vec3 normal;

uniform vec4 materialColor;

void main() {
	fragColor = materialColor * vec4(normal, 1.0);
}