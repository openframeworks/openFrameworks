precision highp float;

// uniform set from a boolean in ofApp
uniform float uUseTextures;

// these are our textures
uniform sampler2D maskTex;

// these are our textures
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;

// this comes from the vertex shader
varying vec2 texCoordVarying;

void main()
{
    
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
    float mask = texture2D(maskTex, texCoordVarying).r;
    
    // sample2D textures are mapped from 0 -> 1 so we do not need to remap
    vec2 st = texCoordVarying;
    
    float mixAmnt = mask;
    // remap the mask value to the 5 different output textures or output colors //
    if( mask < 0.25 ) {
        if(bUseTextures) {
            back = texture2D(tex0, st);
            fore = texture2D(tex1, st);
        } else {
            back = color0;
            fore = color1;
        }
        mixAmnt = mask / 0.25;
    } else if( mask < 0.5 ) {
        if( bUseTextures ) {
            back = texture2D(tex1, st);
            fore = texture2D(tex2, st);
        } else {
            back = color1;
            fore = color2;
        }
        mixAmnt = (mask-0.25) / 0.25;
    } else if( mask < 0.75 ) {
        if( bUseTextures ) {
            back = texture2D(tex2, st);
            fore = texture2D(tex3, st);
        } else {
            back = color2;
            fore = color3;
        }
        mixAmnt = (mask-0.5) / 0.25;
    } else if( mask < 1.0 ) {
        if( bUseTextures ) {
            back = texture2D(tex3, st);
            fore = texture2D(tex4, st);
        } else {
            back = color3;
            fore = color4;
        }
        mixAmnt = (mask-0.75) / 0.25;
    } else {
        if( bUseTextures ) {
            back = texture2D(tex4, st);
        } else {
            back = color4;
        }
        fore = back;
    }
    
    // mix colors from background and foreground based on the mix amount value
    gl_FragColor = mix(back, fore, mixAmnt);
    
}
