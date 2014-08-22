#version 150

uniform sampler2DRect tex0;

uniform float blurAmnt;

in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{

    vec4 color;
	
	color += 1.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmnt * 4.0));
	color += 2.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmnt * 3.0));
	color += 3.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmnt * 2.0));
	color += 4.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmnt * 1.0));
    
	color += 5.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmnt));
	
	color += 4.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmnt * -1.0));
	color += 3.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmnt * -2.0));
	color += 2.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmnt * -3.0));
	color += 1.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmnt * -4.0));

    color /= 25.0;
    
    outputColor = color;
}