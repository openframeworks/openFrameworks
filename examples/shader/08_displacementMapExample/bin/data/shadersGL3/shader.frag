OF_GLSL_SHADER_HEADER

uniform sampler2D tex0;

in vec2 texCoordVarying;

out vec4 outputColor;
 
void main()
{
    outputColor = texture(tex0, texCoordVarying);
}
