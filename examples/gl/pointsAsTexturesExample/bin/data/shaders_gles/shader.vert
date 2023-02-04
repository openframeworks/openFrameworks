#version 300 es
// vertex shader

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec3 normal;
out vec4 color;

void main() {
	gl_Position   = modelViewProjectionMatrix * position;
	color = vec4(1.,0.,0.,1.);
	gl_PointSize  = normal.x;
}
