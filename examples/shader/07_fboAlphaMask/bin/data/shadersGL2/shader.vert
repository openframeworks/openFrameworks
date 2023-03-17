OF_GLSL_SHADER_HEADER

varying vec2 texCoordVarying;

void main()
{
    texCoordVarying = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
}
