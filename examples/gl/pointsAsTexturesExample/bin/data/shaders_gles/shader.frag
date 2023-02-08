#version 300 es
// fragment shader

precision lowp float;

in vec4 in_Color;

out vec4 out_Color;

uniform sampler2D tex;

void main (void) {
	out_Color = texture(tex, gl_PointCoord) * in_Color;
}
