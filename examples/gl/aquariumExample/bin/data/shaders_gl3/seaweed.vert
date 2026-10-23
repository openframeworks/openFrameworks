#version 150
// Ported from the WebGL Aquarium seaweedVertexShader.
// Copyright 2009, Google Inc. BSD-3-Clause.

uniform mat4 world;
uniform mat4 viewProjection;
uniform vec3 lightWorldPos;
uniform mat4 viewInverse;
uniform float time;

in vec4 position;
in vec3 normal;
in vec2 texcoord;

out vec4 v_position;
out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_surfaceToLight;
out vec3 v_surfaceToView;

void main() {
	vec3 toCamera = normalize(viewInverse[3].xyz - world[3].xyz);
	vec3 yAxis = vec3(0.0, 1.0, 0.0);
	vec3 xAxis = cross(yAxis, toCamera);
	mat4 newWorld = mat4(
		vec4(xAxis, 0.0),
		vec4(yAxis, 0.0),
		vec4(xAxis, 0.0),
		world[3]);
	vec4 swayed = position + vec4(
		sin(time * 0.5) * pow(position.y * 0.07, 2.0),
		-4.0,
		0.0,
		0.0);
	v_texCoord = texcoord;
	v_position = (viewProjection * newWorld) * swayed;
	v_normal = (newWorld * vec4(normal, 0.0)).xyz;
	v_surfaceToLight = lightWorldPos - (world * position).xyz;
	v_surfaceToView = (viewInverse[3] - (world * position)).xyz;
	gl_Position = v_position;
}
