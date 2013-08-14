
precision highp float;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D imageMask;

varying vec2 texCoordVarying;

void main()
{
    vec4 rTxt = texture2D(tex0, texCoordVarying);
    vec4 gTxt = texture2D(tex1, texCoordVarying);
    vec4 bTxt = texture2D(tex2, texCoordVarying);
    vec4 mask = texture2D(imageMask, texCoordVarying);
    
    vec4 color = vec4(0, 0, 0, 0);
    color = mix(color, rTxt, mask.r);
    color = mix(color, gTxt, mask.g);
    color = mix(color, bTxt, mask.b);
    
    gl_FragColor = color;
}