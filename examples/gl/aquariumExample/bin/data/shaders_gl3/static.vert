#version 150
// Ported from the WebGL Aquarium. Copyright 2009, Google Inc. BSD-3-Clause.

uniform mat4 viewProjection;
uniform vec3 lightWorldPos;
uniform mat4 world;
uniform mat4 viewInverse;
uniform mat4 worldInverseTranspose;

in vec4 position;
in vec3 normal;
in vec2 texcoord;
in vec3 tangent;
in vec3 binormal;

out vec4 v_position;
out vec2 v_texCoord;
out vec3 v_tangent;
out vec3 v_binormal;
out vec3 v_normal;
out vec3 v_surfaceToLight;
out vec3 v_surfaceToView;

void main() {
	v_texCoord = texcoord;
	v_position = viewProjection * world * position;
	v_normal = (worldInverseTranspose * vec4(normal, 0.0)).xyz;
	v_surfaceToLight = lightWorldPos - (world * position).xyz;
	v_surfaceToView = (viewInverse[3] - (world * position)).xyz;
	v_binormal = (worldInverseTranspose * vec4(binormal, 0.0)).xyz;
	v_tangent = (worldInverseTranspose * vec4(tangent, 0.0)).xyz;
	gl_Position = v_position;
}
