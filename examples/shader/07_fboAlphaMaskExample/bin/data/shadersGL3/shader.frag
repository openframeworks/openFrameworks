OF_GLSL_SHADER_HEADER

// uniform set from a boolean in ofApp
uniform float uUseTextures;
// width and height of the mask texture
uniform vec2 uMaskSize;
// width and height of sample textures
uniform vec2 uTexSize;

uniform sampler2D maskTex;

// these are our textures
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;

// this comes from the vertex shader
in vec2 texCoordVarying;

// this is the output of the fragment shader
out vec4 outputColor;

void main() {
    
    // check the uniform passed in to determine if colors or textures should be used
    bool bUseTextures = uUseTextures > 0.5 ? true : false;
    
    // define some colors to use //
    vec4 color0 = vec4(0.0,0.0,0.1, 1.0);
    vec4 color1 = vec4(0.85, 0.16, 0.43, 1.0);
    vec4 color2 = vec4(0.68, 0.78, 0.6, 1.0);
    vec4 color3 = vec4(1.0, 1.0, 0.0, 1.0);
    vec4 color4 = vec4(1.0, 1.0, 0.6, 1.0);
    
    // create two colors that will be used for mixing
    // give them some default values 
    vec4 back = color0;
    vec4 fore = color1;
    
    // get alpha from mask
    float mask = texture(maskTex, texCoordVarying).r;
    
    // texCoordVarying.x is mapped from 0 -> maskTex width
    // texCoordVarying.y is mapped from 0 -> maskTex height
    // we need to remap it to the size of the texture we want to sample and store it in vec2 st
    vec2 st = vec2(texCoordVarying.x / uMaskSize.x * uTexSize.x,
                   texCoordVarying.y / uMaskSize.y * uTexSize.y);
    
    float mixAmnt = mask;
    // remap the mask value to the 5 different output textures or output colors //
    if( mask < 0.25 ) {
        if(bUseTextures) {
            back = texture(tex0, st);
            fore = texture(tex1, st);
        } else {
            back = color0;
            fore = color1;
        }
        mixAmnt = mask / 0.25;
    } else if( mask < 0.5 ) {
        if( bUseTextures ) {
            back = texture(tex1, st);
            fore = texture(tex2, st);
        } else {
            back = color1;
            fore = color2;
        }
        mixAmnt = (mask-0.25) / 0.25;
    } else if( mask < 0.75 ) {
        if( bUseTextures ) {
            back = texture(tex2, st);
            fore = texture(tex3, st);
        } else {
            back = color2;
            fore = color3;
        }
        mixAmnt = (mask-0.5) / 0.25;
    } else if( mask < 1.0 ) {
        if( bUseTextures ) {
            back = texture(tex3, st);
            fore = texture(tex4, st);
        } else {
            back = color3;
            fore = color4;
        }
        mixAmnt = (mask-0.75) / 0.25;
    } else {
        if( bUseTextures ) {
            back = texture(tex4, st);
        } else {
            back = color4;
        }
        fore = back;
    }
	
	// mix colors from background and foreground based on the mix amount value
    outputColor = mix(back, fore, mixAmnt);
}
