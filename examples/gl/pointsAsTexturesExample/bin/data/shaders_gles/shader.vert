#version 300 es
// vertex shader

uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor;
in vec4 position;
in vec3 normal;

out vec4 in_Color;

void main() {
	gl_Position = modelViewProjectionMatrix * position;
	gl_PointSize  = normal.x;
	in_Color = globalColor;
}
