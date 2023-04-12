#version 330

// ------------------------
// set automatically by OF //
in vec4 position;
in vec3 normal;
in vec4 color;
in vec2 texcoord;

uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor;
uniform float usingColors;
// ------------------------

out vec3 positionFromVertex;
out vec3 normalFromVertex;
out vec4 colorFromVertex;

void main()
{
	colorFromVertex = color;
	if( usingColors < 1.0 ) {
		colorFromVertex = globalColor;
	}
	normalFromVertex = normal;
	positionFromVertex = position.xyz;
	gl_Position = position;
}
