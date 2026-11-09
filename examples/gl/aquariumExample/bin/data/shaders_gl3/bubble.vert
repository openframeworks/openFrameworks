#version 150

uniform mat4 viewProjection;

in vec4 position;
in vec2 texcoord;
in vec4 color;

out vec2 v_texCoord;
out vec4 v_color;

void main() {
	v_texCoord = texcoord;
	v_color = color;
	gl_Position = viewProjection * position;
}
