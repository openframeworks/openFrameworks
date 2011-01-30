
uniform sampler2D	tex;
uniform float		angle;


void main (void) {
	
	vec2 offset		= vec2(.5, .5);
	float rotspeed  = 3.0;
	vec2 v2 = gl_TexCoord[0].st;
	
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
	gl_FragDepth = gl_FragCoord.z;
}