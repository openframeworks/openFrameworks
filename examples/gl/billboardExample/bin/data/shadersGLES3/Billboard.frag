#version 300 es

precision lowp float;

uniform sampler2D tex;

in vec4 colorVarying;

out vec4 fragColor;

void main (void) {
	fragColor = texture(tex, gl_PointCoord) * colorVarying;
}
