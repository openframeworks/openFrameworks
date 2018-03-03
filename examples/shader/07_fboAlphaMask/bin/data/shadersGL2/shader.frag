#version 120

// these are our textures
uniform sampler2DRect tex0;//this is the background texture
uniform sampler2DRect maskTex;
uniform sampler2DRect foregroundTex;

// this comes from the vertex shader
varying vec2 texCoordVarying;

void main()
{
    // Get color values from the background and foreground
    vec4 back = texture2DRect(tex0, texCoordVarying);
    vec4 fore = texture2DRect(foregroundTex, texCoordVarying);

	// get alpha from mask
    float mask = texture2DRect(maskTex, texCoordVarying).r;

	//mix colors from background and foreground based on the mask value
    gl_FragColor = mix(fore , back, mask);
}