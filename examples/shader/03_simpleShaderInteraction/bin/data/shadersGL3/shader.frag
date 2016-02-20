#version 150

out vec4 outputColor;

uniform vec4 mouseColor;
 
void main()
{
    outputColor = mouseColor;
}