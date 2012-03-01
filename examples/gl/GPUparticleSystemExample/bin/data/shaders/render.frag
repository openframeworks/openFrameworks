#version 120
#extension GL_ARB_texture_rectangle : enable

// This fill the billboard made on the Geometry Shader with a texture

uniform sampler2DRect sparkTex;

void main() {
    vec2 st = gl_TexCoord[0].st;
    gl_FragColor = texture2DRect(sparkTex, st);
}