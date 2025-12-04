OF_GLSL_SHADER_HEADER

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D imageMask;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    vec4 rTxt = texture(tex0, texCoordVarying);
    vec4 gTxt = texture(tex1, texCoordVarying);
    vec4 bTxt = texture(tex2, texCoordVarying);
    vec4 mask = texture(imageMask, texCoordVarying);
    
    vec4 color = vec4(0, 0, 0, 0);
    color = mix(color, rTxt, mask.r);
    color = mix(color, gTxt, mask.g);
    color = mix(color, bTxt, mask.b);
    
    outputColor = color;
}
