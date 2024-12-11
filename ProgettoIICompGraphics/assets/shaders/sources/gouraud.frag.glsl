#version 330 core

in vec4 lightingColor;
in vec2 uvIn;

out vec4 fragColor;

uniform sampler2D albedo0;

void main() {
    vec4 textureColor = texture(albedo0, uvIn);
    fragColor = lightingColor * textureColor;
}
