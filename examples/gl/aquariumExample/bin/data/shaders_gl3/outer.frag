#version 150
// Ported from the WebGL Aquarium outerRefractionMapFragmentShader.
// Copyright 2009, Google Inc. BSD-3-Clause.

uniform float uNormal;
uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D reflectionMap;
uniform samplerCube skybox;

in vec2 v_texCoord;
in vec3 v_tangent;
in vec3 v_binormal;
in vec3 v_normal;
in vec3 v_surfaceToView;

out vec4 fragColor;

void main() {
	vec4 diffuseColor = texture(diffuse, v_texCoord);
	vec4 reflection = texture(reflectionMap, v_texCoord);
	vec3 normal = normalize(v_normal);
	if (uNormal > 0.5) {
		vec4 normalSpec = texture(normalMap, v_texCoord);
		vec3 tangentNormal = normalSpec.xyz - vec3(0.5);
		mat3 tangentToWorld = mat3(v_tangent, v_binormal, v_normal);
		normal = normalize(tangentToWorld * tangentNormal);
	}
	vec3 surfaceToView = normalize(v_surfaceToView);
	vec4 skyColor = texture(skybox, -reflect(surfaceToView, normal));
	float fudgeAmount = 1.1;
	vec3 fudge = skyColor.rgb * vec3(fudgeAmount);
	float bright = min(1.0, fudge.r * fudge.g * fudge.b);
	vec4 reflectColor = mix(vec4(skyColor.rgb, bright), diffuseColor, 1.0 - reflection.r);
	float r = abs(dot(surfaceToView, normal));
	fragColor = vec4(mix(skyColor, reflectColor, (r + 0.3) * reflection.r).rgb, 1.0 - r);
}
