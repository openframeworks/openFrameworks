OF_GLSL_SHADER_HEADER

#ifndef PI
#define PI 3.14159265359
#endif

//-- automatically set by OF ------- //
uniform mat4 modelViewProjectionMatrix;
// ---------------------------------- //

// Vertex Attributes Coming in ------ //
in vec3 position;
in vec4 color;
// ---------------------------------- //

//-- uniforms set from ofApp ---- //
uniform float uPointSize;
uniform float uElapsedTime;

uniform vec4 uStartColor1;
uniform vec4 uStartColor2;
uniform vec4 uEndColor;
// ---------------------------------- //

//-- output to the frag shader ---- //
out vec4 oColor;
// ---------------------------------- //

void main(){
	gl_Position = modelViewProjectionMatrix * vec4( position.xyz, 1.0 );
	
	float plife = color.z;
	float maxLife = color.w;
	float lifePercent = (plife/maxLife);
	
	float lifeSine = sin( lifePercent * PI );
	float sizeUnique = fract(color.y/9.3083f);
	gl_PointSize = (lifeSine + (1.f-lifePercent)) * uPointSize * (sizeUnique*2.0) + 1.+(sizeUnique * uPointSize * 0.1);
	
	vec4 startColor = uStartColor1;
	vec4 endColor = uEndColor;
	
	float colorUnique = fract( color.x / 18.49 );
	if( colorUnique > 0.5 ) {
		startColor = uStartColor2;
	}
	
	oColor = mix( startColor, endColor, pow(lifePercent, 1.25) );
	oColor.a = oColor.a * lifeSine;
}
