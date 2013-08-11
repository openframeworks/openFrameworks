#version 150

in vec2 texCoordVarying;
out vec4 outputColor;

uniform sampler2DRect tex0;
uniform sampler2DRect imageMask;

void main()
{
    vec4 texel0 = texture(tex0, texCoordVarying);
    vec4 texel1 = texture(imageMask, texCoordVarying);
    outputColor = vec4(texel0.rgb, texel0.a * texel1.a);
}