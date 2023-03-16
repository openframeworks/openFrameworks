#version 120

// this is how we receive the texture
uniform sampler2D tex0;
uniform vec2 resolution;

void main()
{
    gl_FragColor = texture2D(tex0, gl_FragCoord.xy / resolution);
}
