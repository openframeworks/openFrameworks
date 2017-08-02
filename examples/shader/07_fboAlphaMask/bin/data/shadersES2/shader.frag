precision highp float;

// these are our textures
uniform sampler2D tex0;//this is the background texture
uniform sampler2D maskTex;
uniform sampler2D foregroundTex;

// this comes from the vertex shader
varying vec2 texCoordVarying;

void main()
{
	// Get color values from the background and foreground
	vec4 back = texture2D(tex0, texCoordVarying);
	vec4 fore = texture2D(foregroundTex, texCoordVarying);

	// get alpha from mask
	float mask = texture2D(maskTex, texCoordVarying).r;
	
	//mix colors from background and foreground based on the mask value
	gl_FragColor = mix(fore , back, mask);
}