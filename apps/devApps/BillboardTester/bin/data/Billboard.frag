
uniform sampler2D tex;
uniform float timer;
uniform vec2 rot;

void main (void) {
	
	vec2 offset = vec2(.5, .5);
	
	float rotspeed = 3.0;
	vec2 v2 = gl_TexCoord[0].st;
	
	float cosX, sinX;
	float cosY, sinY;
	
	sinX = sin(rotspeed*timer);
	cosX = cos(rotspeed*timer);
	
	sinY = sin(rotspeed*timer);
	cosY = cos(rotspeed*timer);
	
	mat2 rotationMatrix = mat2( cosX, -sinX,
							    sinY, cosX);
	
	float scaletimer = timer;
	vec2 newcoords = ((v2-offset) * (rotationMatrix));
	newcoords += offset;
	
	
	
	gl_FragColor = texture2D(tex, newcoords) * gl_Color;
	gl_FragDepth = gl_FragCoord.z;
}