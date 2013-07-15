#version 150
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect posTex;
uniform vec2 screen;
uniform float resolution;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec2  texcoord;

out vec2 texCoordVarying;

void main() {
    // use the texture coordinates as an index into the position texture
    vec2 verPos = texcoord;

    // read position data from texture
    vec4 pixPos = texture( posTex, verPos );
    
    // Maps the position from the texture (from 0.0 to 1.0) to
    // the screen position (0 - screenWidth/screenHeight)
    //
    pixPos.x *= screen.x;
    pixPos.y *= screen.y;
    
    gl_Position = pixPos;
	texCoordVarying = texcoord;
}