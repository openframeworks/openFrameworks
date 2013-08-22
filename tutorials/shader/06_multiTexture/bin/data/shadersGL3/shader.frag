#version 150

uniform sampler2DRect redTex;
uniform sampler2DRect greenTex;
uniform sampler2DRect blueTex;
uniform sampler2DRect imageMask;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    vec4 rTxt = texture(redTex, texCoordVarying);
    vec4 gTxt = texture(greenTex, texCoordVarying);
    vec4 bTxt = texture(blueTex, texCoordVarying);
    vec4 mask = texture(imageMask, texCoordVarying);

    outputColor = vec4(rTxt.r, gTxt.g, bTxt.b, mask.r);
}