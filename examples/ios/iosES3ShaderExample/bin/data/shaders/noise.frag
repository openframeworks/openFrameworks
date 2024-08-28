#version 300 es

#ifdef GL_ES
    precision highp float;
#endif

uniform sampler2D src_tex_unit0;
uniform float useTexture;
uniform float useColors;
uniform vec4 globalColor;

in float depth;             // Replaces varying keyword from ES2
in vec4 colorVarying;
in vec2 texCoordVarying;

out vec4 fragColor;         // Replaces gl_FragColor in ES3

void main() {
    float xVal = gl_FragCoord.x;
    float yVal = gl_FragCoord.y;
    
    vec4 c;
    if (useColors > 0.5) {
        c = colorVarying;
    } else {
        c = globalColor;
    }

    // Apply a pattern based on the fragment's x and y position
    if (mod(xVal, 4.0) > 1.0 && mod(yVal, 4.0) > 1.0) {
        // Leave unchanged
    } else {
        c.a = 0.2;  // Reduce alpha for certain pixels
    }
    
    if (useTexture > 0.5) {
        // Use texture() instead of texture2D() in ES3
        fragColor = texture(src_tex_unit0, texCoordVarying) * c;
    } else {
        fragColor = c;
    }
}
