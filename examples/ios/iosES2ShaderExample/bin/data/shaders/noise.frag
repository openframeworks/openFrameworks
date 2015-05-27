#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

uniform sampler2D src_tex_unit0;
uniform float useTexture;
uniform float useColors;
uniform vec4 globalColor;

varying float depth;
varying vec4 colorVarying;
varying vec2 texCoordVarying;

void main(){
	//this is the fragment shader
	//this is where the pixel level drawing happens
	//gl_FragCoord gives us the x and y of the current pixel its drawing
	
	//we grab the x and y and store them in an int
	float xVal = gl_FragCoord.x;
	float yVal = gl_FragCoord.y;
    
	vec4 c;
	if(useColors>0.5){
		c = colorVarying;
	}else{
		c = globalColor;
	}

	if(mod(xVal, 4.0) > 1.0 && mod(yVal, 4.0) > 1.0){
		// leave unchanged
    }else{
		c.a = 0.2;
	}
	
    if(useTexture>0.5){
		gl_FragColor = texture2D(src_tex_unit0, texCoordVarying) * c;
	}else{
		gl_FragColor = c;
	}
}