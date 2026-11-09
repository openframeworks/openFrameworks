#version 150

uniform sampler2D colorMap;

in vec2 v_texCoord;
in vec4 v_color;
out vec4 fragColor;

void main() {
	fragColor = texture(colorMap, v_texCoord) * v_color;
}
