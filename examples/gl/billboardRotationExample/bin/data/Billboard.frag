
uniform sampler2D	tex;
varying  float		rotAngle;


void main (void) {
	
	float angle		= rotAngle;
	vec2 offset		= vec2(.5, .5);
	vec2 v2 = gl_TexCoord[0].st;	
	
	// not sure this is right
	// I just search on how to rotate
	// a texture...
	float cosX, sinX;
	float cosY, sinY;
	sinX = sin(angle);
	cosX = cos(angle);
	sinY = sin(angle);
	cosY = cos(angle);
	
	mat2 rotationMatrix = mat2( cosX, -sinX,
							    sinY, cosX);
	
	vec2 newcoords = ((v2-offset) * (rotationMatrix));
	newcoords += offset;
	
	
	
	gl_FragColor = texture2D(tex, newcoords) * gl_Color;
}