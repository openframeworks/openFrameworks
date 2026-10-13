#version 150

uniform vec4 colorMult;
uniform sampler2D colorMap;

in vec2 v_texCoord;
out vec4 fragColor;

void main() {
	fragColor = texture(colorMap, v_texCoord) * colorMult;
}
