#version 330 core

out vec4 fragColor;

in vec3 uv;

uniform samplerCube material_skybox;

void main() {
    fragColor = texture(material_skybox, uv);
}