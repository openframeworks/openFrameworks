OF_GLSL_SHADER_HEADER
uniform sampler2D tex;

in vec4 in_Color;

out vec4 out_Color;

void main (void) {
    
    out_Color = texture(tex, gl_PointCoord) * in_Color;
    
}
