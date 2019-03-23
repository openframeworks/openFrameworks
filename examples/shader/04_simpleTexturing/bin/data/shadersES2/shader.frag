
precision highp float;

uniform vec2 resolution;
uniform sampler2D tex0;
uniform float mouseX;

void main()
{
    gl_FragColor = texture2D(tex0, (gl_FragCoord.xy + vec2(mouseX,0))/resolution.xy);
}