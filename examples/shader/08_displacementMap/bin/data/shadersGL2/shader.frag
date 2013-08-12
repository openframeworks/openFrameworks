#version 120

uniform sampler2DRect tex0;				

void main()
{
    gl_FragColor = texture2DRect(tex0, gl_TexCoord[0].st);
}