
precision highp float;

uniform sampler2D redTex;
uniform sampler2D greenTex;
uniform sampler2D blueTex;
uniform sampler2D imageMask;

varying vec2 texCoordVarying;

void main()
{
    vec4 rTxt = texture2D(redTex, texCoordVarying);
    vec4 gTxt = texture2D(greenTex, texCoordVarying);
    vec4 bTxt = texture2D(blueTex, texCoordVarying);
    vec4 mask = texture2D(imageMask, texCoordVarying);
    
    gl_FragColor = vec4(rTxt.r, gTxt.g, bTxt.b, mask.r);;
}