#version 150

// these come from the programmable pipeline
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

// texture coordinates are sent to fragment shader
out vec2 texCoordVarying;

void main()
{
    texCoordVarying = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}