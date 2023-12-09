#version 120

varying vec3 vnormal;

void main()
{
	gl_FrontColor =  gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_Vertex;
//	vnormal = vec3(10,0,0);//gl_Normal;
	vnormal = gl_Normal;
}
