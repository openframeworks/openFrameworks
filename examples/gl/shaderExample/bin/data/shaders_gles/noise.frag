#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

uniform vec4 globalColor;

void main(){
	//this is the fragment shader
	//this is where the pixel level drawing happens
	//gl_FragCoord gives us the x and y of the current pixel its drawing
	
	//we grab the x and y and store them in an int
	float xVal = gl_FragCoord.x;
	float yVal = gl_FragCoord.y;
    
	
	//we use the mod function to only draw pixels if they are every 2 in x or every 4 in y
	if( mod(xVal, 2.0) == 0.5 && mod(yVal, 4.0) == 0.5 ){
		gl_FragColor = globalColor;    
    }else{
		discard;
	}
	
}
