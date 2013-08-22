#version 120

uniform sampler2DRect redTex;
uniform sampler2DRect greenTex;
uniform sampler2DRect blueTex;
uniform sampler2DRect imageMask;

varying vec2 texCoordVarying;

void main()
{
    vec4 rTxt = texture2DRect(redTex, texCoordVarying);
    vec4 gTxt = texture2DRect(greenTex, texCoordVarying);
    vec4 bTxt = texture2DRect(blueTex, texCoordVarying);
    vec4 mask = texture2DRect(imageMask, texCoordVarying);
    
    gl_FragColor = vec4(rTxt.r, gTxt.g, bTxt.b, mask.r);
}