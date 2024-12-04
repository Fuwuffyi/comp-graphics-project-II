#version 330 core

out vec4 fragColor;

uniform vec4 materialColor;

void main() {
	fragColor = materialColor;
}