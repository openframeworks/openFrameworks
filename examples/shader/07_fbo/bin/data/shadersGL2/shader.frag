#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;

void main()
{
// Get texture coordinates
vec2 pos = gl_TexCoord[0].st;

// Get color value from 
vec3 src = texture2DRect(tex0, pos).rgb;

// Get alpha value
float mask = texture2DRect(maskTex, pos).r;

// Set
gl_FragColor = vec4( src , mask);
}