#version 150

// these are our textures
uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;

// this comes from the vertex shader
in vec2 texCoordVarying;

// this is the output of the fragment shader
out vec4 outputColor;

void main()
{
    // get rgb from tex0
    vec3 src = texture(tex0, texCoordVarying).rgb;

    // get alpha from mask
    float mask = texture(maskTex, texCoordVarying).r;
    
    //mix the rgb from tex0 with the alpha of the mask
    outputColor = vec4(src , mask);
}