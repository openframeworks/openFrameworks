#version 150

// This fill the billboard made on the Geometry Shader with a texture

uniform sampler2DRect sparkTex;

in vec2 vTexCoord;

out vec4 vFragColor;

void main() {
    vFragColor = texture(sparkTex, vTexCoord);
}