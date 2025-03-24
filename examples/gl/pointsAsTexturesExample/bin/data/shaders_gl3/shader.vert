OF_GLSL_SHADER_HEADER

uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor;
in vec4 position;
in vec3 normal;
in vec4 color;

out vec4 in_Color;

uniform float pointSize;

void main() {
    gl_Position = modelViewProjectionMatrix * position;
	// we are passing in size variations via the normal.x
	gl_PointSize  = normal.x * pointSize;
	in_Color = color;

}
