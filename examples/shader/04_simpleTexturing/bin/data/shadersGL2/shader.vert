#version 120

uniform float inValue;

void main()
{
	vec4 pos = ftransform();
	pos.y += (sin(pos.x + (inValue * 2.)));
	gl_Position = pos;
}