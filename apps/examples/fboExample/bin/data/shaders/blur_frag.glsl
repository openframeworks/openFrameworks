#version 120
#extension GL_ARB_texture_rectangle: enable

uniform sampler2DRect tex0;
uniform float sampleOffset;

void main(void) 
{
    vec4 center = texture2DRect(tex0, gl_TexCoord[0].st);
	
	vec4 sum = center * 4.0;
    sum	+= texture2DRect(tex0, gl_TexCoord[0].st + vec2(0.0, -sampleOffset)) * 2.0;
    sum	+= texture2DRect(tex0, gl_TexCoord[0].st + vec2(0.0, sampleOffset)) * 2.0;
    sum	+= texture2DRect(tex0, gl_TexCoord[0].st + vec2(-sampleOffset, 0.0)) * 2.0;
    sum	+= texture2DRect(tex0, gl_TexCoord[0].st + vec2(sampleOffset, 0.0)) * 2.0;

    sum	+= texture2DRect(tex0, gl_TexCoord[0].st + vec2(-sampleOffset, -sampleOffset));
    sum	+= texture2DRect(tex0, gl_TexCoord[0].st + vec2(sampleOffset, -sampleOffset));
    sum	+= texture2DRect(tex0, gl_TexCoord[0].st + vec2(sampleOffset, sampleOffset));
    sum	+= texture2DRect(tex0, gl_TexCoord[0].st + vec2(-sampleOffset, sampleOffset));

    gl_FragColor = sum / 16.0;
}
