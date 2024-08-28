#version 330

/***********************************************
 Geometry shader to convert lines into triangle strips
 Memo Akten
 www.memo.tv

 Updated by Nick Hardeman :)
************************************************/

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

// ------------------------
// set automatically by OF //
uniform mat4    modelViewProjectionMatrix;
// ------------------------

uniform float thickness;
uniform vec3 lightDir;
uniform float uUseNormals;

// [2] because this makes a line
in vec3 positionFromVertex[2];
in vec3 normalFromVertex[2];
in vec4 colorFromVertex[2];

out vec4 colorForFrag;

void main() {
	vec3 p0 = positionFromVertex[0].xyz;
	vec3 p1 = positionFromVertex[1].xyz;
	
	vec3 up = vec3(0, 0, 1);	// arbitrary up vector
	
	vec3 dir = normalize(p1 - p0);			// normalized direction vector from p0 to p1
	vec3 right = normalize(cross(dir, up));	// right vector
	
	vec3 norm = cross(right, dir);
	float fColMult = pow(abs(dot(norm, lightDir)) * 0.75 + 0.25, 0.6);
	vec4 colMult = vec4(fColMult, fColMult, fColMult, 1.0);
	
	if(uUseNormals > 0.5) {
		right = (normalFromVertex[0]);
	}
	vec3 nextRight = right;
	if( uUseNormals > 0.5 ) {
		nextRight = (normalFromVertex[1]);
	}
	
	right *= thickness;
	nextRight *= thickness;
	
	
	gl_Position = modelViewProjectionMatrix * vec4(p0 - right, 1.0);
	colorForFrag = colorFromVertex[0] * colMult;
	EmitVertex();
	
	gl_Position = modelViewProjectionMatrix * vec4(p0 + right, 1.0);
	colorForFrag = colorFromVertex[0] * colMult;
	EmitVertex();
	
	gl_Position = modelViewProjectionMatrix * vec4(p1 - nextRight, 1.0);
	colorForFrag = colorFromVertex[0] * colMult;
	EmitVertex();

	gl_Position = modelViewProjectionMatrix * vec4(p1 + nextRight, 1.0);
	colorForFrag = colorFromVertex[0] * colMult;
	EmitVertex();

}
