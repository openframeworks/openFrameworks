
attribute vec4 position;
attribute vec2 texcoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 textureMatrix;

varying vec2 texCoordVarying;

void main()
{
    texCoordVarying = (textureMatrix * vec4(texcoord.x, texcoord.y, 0, 1)).xy;
	gl_Position = modelViewProjectionMatrix * position;
}