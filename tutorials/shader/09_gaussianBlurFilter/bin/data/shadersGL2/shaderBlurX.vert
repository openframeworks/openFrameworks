#version 120

varying vec2 texCoordVarying;

void main(void)
{
	texCoordVarying = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
}