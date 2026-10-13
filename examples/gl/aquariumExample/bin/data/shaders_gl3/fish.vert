#version 150
// Ported from the WebGL Aquarium fishVertexShader.
// Copyright 2009, Google Inc. BSD-3-Clause.

uniform vec3 lightWorldPos;
uniform mat4 viewInverse;
uniform mat4 viewProjection;
uniform vec3 worldPosition;
uniform vec3 nextPosition;
uniform float scale;
uniform float time;
uniform float fishLength;
uniform float fishWaveLength;
uniform float fishBendAmount;

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
	vec3 vz = normalize(worldPosition - nextPosition);
	vec3 vx = normalize(cross(vec3(0.0, 1.0, 0.0), vz));
	vec3 vy = cross(vz, vx);
	mat4 orientMat = mat4(
		vec4(vx, 0.0),
		vec4(vy, 0.0),
		vec4(vz, 0.0),
		vec4(worldPosition, 1.0));
	mat4 scaleMat = mat4(
		vec4(scale, 0.0, 0.0, 0.0),
		vec4(0.0, scale, 0.0, 0.0),
		vec4(0.0, 0.0, scale, 0.0),
		vec4(0.0, 0.0, 0.0, 1.0));
	mat4 world = orientMat * scaleMat;

	v_texCoord = texcoord;
	float mult = position.z > 0.0 ? (position.z / fishLength) : (-position.z / fishLength * 2.0);
	float s = sin(time + mult * fishWaveLength);
	float offset = pow(mult, 2.0) * s * fishBendAmount;
	vec4 bent = position + vec4(offset, 0.0, 0.0, 0.0);
	v_position = viewProjection * world * bent;
	v_normal = (world * vec4(normal, 0.0)).xyz;
	v_surfaceToLight = lightWorldPos - (world * position).xyz;
	v_surfaceToView = (viewInverse[3] - (world * position)).xyz;
	v_binormal = (world * vec4(binormal, 0.0)).xyz;
	v_tangent = (world * vec4(tangent, 0.0)).xyz;
	gl_Position = v_position;
}
