#version 300 es
// fragment shader

precision lowp float;

in vec4 color;
out vec4 out_Color;

uniform sampler2D tex;

void main (void) {
        out_Color = vec4(color.rgb, texture(tex, gl_PointCoord).a);
}
