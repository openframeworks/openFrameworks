#version 150

// these are our textures
uniform sampler2DRect tex0;//this is the background texture
uniform sampler2DRect maskTex;
uniform sampler2DRect foregroundTex;

// this comes from the vertex shader
in vec2 texCoordVarying;

// this is the output of the fragment shader
out vec4 outputColor;

void main()
{
	// Get color values from the background and foreground
	vec4 back = texture(tex0, texCoordVarying);
	vec4 fore = texture(foregroundTex, texCoordVarying);

    // get alpha from mask
    float mask = texture(maskTex, texCoordVarying).r;
	
	//mix colors from background and foreground based on the mask value
    outputColor = mix(fore , back, mask);
}
