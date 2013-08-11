#version 120

// this is how we receive the texture
uniform sampler2DRect tex0;

void main()
{
    vec4 src = texture2DRect(tex0, gl_TexCoord[0].st);
    gl_FragColor = src;
}