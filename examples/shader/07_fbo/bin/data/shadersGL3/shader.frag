#version 150

// this comes from the vertex shader
in vec2 texCoordVarying;

// this is the output of the fragment shader
out vec4 outputColor;

// these are our textures
uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;

void main()
{
    // get texture coordinates
    vec2 pos = texCoordVarying;

    // get rgb from tex0
    vec3 src = texture(tex0, pos).rgb;

    // get alpha from mask
    float mask = texture(maskTex, pos).r;
    
    //mix the rgb from tex0 with the alpha of the mask
    outputColor = vec4( src , mask);
}