#version 120
#extension GL_ARB_texture_rectangle : enable

#pragma include "ofBlendingUtils.frag"
#pragma include "ofImageProcessingUtils.frag"

uniform sampler2DRect tex0;
uniform float blurScale;
uniform float saturation;

void main(){
	vec2 st	= gl_TexCoord[0].st;
	
	//we can grab the original color
	vec4 ogColor = texture2DRect(tex0, st);
	
	//or we can get a blur 
	//note this is just for testing - a good blur would do it with ping-ponging fbos. 
	vec4 blurColor	=  blurH(tex0, st, blurScale); 
	blurColor		+= blurV(tex0, st, blurScale); 
	blurColor *= 0.5; 

	//lets do another blur at a different scale to soften it a bit
	blurColor		+= blurV(tex0, st, blurScale*2.2) * 0.7; 
	blurColor		+= blurH(tex0, st, blurScale*2.2) * 0.7; 

	blurColor.rgb = ContrastSaturationBrightness(blurColor.rgb, 1.0, saturation, 1.5);
	
	gl_FragColor = blurColor;
}