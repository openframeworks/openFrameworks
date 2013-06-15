#version 150

// This fill the billboard made on the Geometry Shader with a texture

uniform sampler2DRect sparkTex;
in vec2 texCoordVarying;
out vec4 fragColor;

void main() {
    vec2 st = texCoordVarying;
    fragColor = texture(sparkTex, st);
}