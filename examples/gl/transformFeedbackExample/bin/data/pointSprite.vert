#version 330

//-- automatically set by OF ------- //
uniform mat4 modelViewProjectionMatrix;
// ---------------------------------- //

// Vertex Attributes Coming in ------ //
in vec3 position;
in vec4 color;
// ---------------------------------- //

//-- uniforms set from ofApp ---- //
uniform float uPointSize = 10;
// ---------------------------------- //

//-- output to the frag shader ---- //
out vec4 oColor;
// ---------------------------------- //

void main(){
	gl_Position = modelViewProjectionMatrix * vec4( position.xyz, 1.0 );
	gl_PointSize = uPointSize;
	
	oColor = color;
}
