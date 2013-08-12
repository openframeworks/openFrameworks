#version 120

void main()
{
    texCoordVarying = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
}