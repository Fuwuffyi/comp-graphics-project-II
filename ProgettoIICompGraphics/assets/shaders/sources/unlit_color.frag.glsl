#version 330 core

out vec4 fragColor;

uniform vec3 material_color;

void main() {
	fragColor = vec4(material_color, 1.0);
}