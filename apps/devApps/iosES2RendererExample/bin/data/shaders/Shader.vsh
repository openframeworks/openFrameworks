attribute vec4 position;
attribute vec4 color;

uniform mat4 modelViewProjectionMatrix;

varying vec4 colorVarying;

void main()
{
	gl_Position = modelViewProjectionMatrix * position;
	colorVarying = color;
}
