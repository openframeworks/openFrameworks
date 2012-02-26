
attribute float		pointSize;
attribute  float	angle;
varying  float		rotAngle;
void main() {
	
	rotAngle = angle;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec4 eyeCoord  = gl_ModelViewMatrix * gl_Vertex;
	gl_Position    = gl_ProjectionMatrix * eyeCoord;
	float dist     = sqrt(eyeCoord.x*eyeCoord.x + eyeCoord.y*eyeCoord.y + eyeCoord.z*eyeCoord.z);
	float att	   = 600.0 / dist;
	gl_PointSize   = pointSize * att;	
	gl_FrontColor  = gl_Color;
	
	
}





