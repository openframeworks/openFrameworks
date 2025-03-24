OF_GLSL_SHADER_HEADER

uniform sampler2D tex0;
uniform sampler2D imageMask;

varying vec2 texCoordVarying;

void main()
{
    vec4 texel0 = texture2D(tex0, texCoordVarying);
    vec4 texel1 = texture2D(imageMask, texCoordVarying);
    gl_FragColor = vec4(texel0.rgb, texel0.a * texel1.a);
}
