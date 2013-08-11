#version 150

// this is how we receive the texture
uniform sampler2DRect tex0;


in vec2 varyingtexcoord;

out vec4 outputColor;
 
void main()
{
    outputColor = texture(tex0, varyingtexcoord);
}