
uniform float particleSize;
uniform vec4 theColor;

void main() {
	
	//vec4 c = vec4(theColor[0], theColor[1], theColor[2], 0.3);
	//gl_FrontColor = c;
	//gl_Position  = ftransform();	
	//gl_PointSize = thePixelSize;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex;
	gl_Position = gl_ProjectionMatrix * eyeCoord;
	float dist = sqrt(eyeCoord.x*eyeCoord.x + eyeCoord.y*eyeCoord.y + eyeCoord.z*eyeCoord.z);
	float att = 600.0 / dist;
	
	gl_PointSize = particleSize * att;
	gl_FrontColor = theColor;
	
	
}





