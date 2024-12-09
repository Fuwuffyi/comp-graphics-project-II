#version 330 core

out vec4 fragColor;

uniform vec3 materialColor;

void main() {
	fragColor = vec4(materialColor, 1.0);
}