#version 330

in vec4 colorForFrag;

out vec4 FRAGCOLOR;

void main (void)  
{  
	FRAGCOLOR = colorForFrag;
}
