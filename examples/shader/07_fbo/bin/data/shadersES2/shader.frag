
precision highp float;

in vec2 texCoordVarying;
out vec4 outputColor;

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;

void main()
{
    vec2 pos = texCoordVarying;
    vec3 src = texture(tex0, pos).rgb;
    float mask = texture(maskTex, pos).r;
    outputColor = vec4( src , mask);
}