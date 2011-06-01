#version 120
#extension GL_ARB_draw_buffers: enable

void main(void) 
{
	gl_FragData[0] = gl_Color;
	gl_FragData[1] = vec4(vec3(1.0) - gl_Color.rgb, gl_Color.a);
	gl_FragData[2] = vec4(gl_Color.rgb/2.0, gl_Color.a);
	gl_FragData[3] = vec4(gl_Color.rgb * 2.0, gl_Color.a);
}
