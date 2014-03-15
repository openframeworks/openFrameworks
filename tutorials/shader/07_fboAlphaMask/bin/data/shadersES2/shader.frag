
precision highp float;

uniform sampler2D tex0;
uniform sampler2D maskTex;

varying vec2 texCoordVarying;

void main()
{
    vec3 src = texture2D(tex0, texCoordVarying).rgb;
    float mask = texture2D(maskTex, texCoordVarying).r;
    gl_FragColor = vec4(src , mask);
}