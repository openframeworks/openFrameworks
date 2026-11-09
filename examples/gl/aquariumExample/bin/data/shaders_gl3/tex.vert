#version 150

uniform mat4 world;
uniform mat4 viewProjection;

in vec4 position;
in vec2 texcoord;

out vec2 v_texCoord;

void main() {
	v_texCoord = texcoord;
	gl_Position = viewProjection * world * position;
}
