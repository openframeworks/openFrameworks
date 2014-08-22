#version 120

uniform sampler2DRect tex0;

uniform float blurAmnt;

varying vec2 texCoordVarying;

void main()
{
    vec4 color;
	
    color += 1.0 * texture2DRect(tex0, texCoordVarying + vec2(blurAmnt * -4.0, 0.0));
    color += 2.0 * texture2DRect(tex0, texCoordVarying + vec2(blurAmnt * -3.0, 0.0));
    color += 3.0 * texture2DRect(tex0, texCoordVarying + vec2(blurAmnt * -2.0, 0.0));
    color += 4.0 * texture2DRect(tex0, texCoordVarying + vec2(blurAmnt * -1.0, 0.0));
    
    color += 5.0 * texture2DRect(tex0, texCoordVarying + vec2(blurAmnt, 0));
	
    color += 4.0 * texture2DRect(tex0, texCoordVarying + vec2(blurAmnt * 1.0, 0.0));
    color += 3.0 * texture2DRect(tex0, texCoordVarying + vec2(blurAmnt * 2.0, 0.0));
    color += 2.0 * texture2DRect(tex0, texCoordVarying + vec2(blurAmnt * 3.0, 0.0));
    color += 1.0 * texture2DRect(tex0, texCoordVarying + vec2(blurAmnt * 4.0, 0.0));
	
    color /= 25.0;
    
    gl_FragColor = color;
}