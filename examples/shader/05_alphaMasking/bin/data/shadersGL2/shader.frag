#version 120

varying vec2 texCoordVarying;

uniform sampler2DRect tex0;
uniform sampler2DRect imageMask;

void main()
{
    vec4 texel0 = texture2DRect(tex0, texCoordVarying);
    vec4 texel1 = texture2DRect(imageMask, texCoordVarying);
    gl_FragColor = vec4(texel0.rgb, texel0.a * texel1.a);
}