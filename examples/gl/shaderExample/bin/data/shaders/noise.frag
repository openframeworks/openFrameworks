#version 120

void main(){
	//this is the fragment shader
	//this is where the pixel level drawing happens
	//gl_FragCoord gives us the x and y of the current pixel its drawing
	
	//we grab the x and y and store them in an int
	int xVal = int(gl_FragCoord.x);
	int yVal = int(gl_FragCoord.y);
	
	//we use the mod function to only draw pixels if they are every 2 in x or every 4 in y
	if( mod(xVal, 2) == 0 && mod(yVal, 4) == 0 ){
		gl_FragColor = gl_Color;    
    }
	
}