#version 150
// Ported from the WebGL Aquarium diffuse / normal / reflection fragments.
// Copyright 2009, Google Inc. BSD-3-Clause.

uniform vec4 lightColor;
uniform vec4 ambient;
uniform vec4 specular;
uniform float shininess;
uniform float specularFactor;
uniform float uNormal;
uniform float uReflect;
uniform float uNormalBias;
uniform float uFog;
uniform float fogPower;
uniform float fogMult;
uniform float fogOffset;
uniform vec4 fogColor;
uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D reflectionMap;
uniform samplerCube skybox;

in vec4 v_position;
in vec2 v_texCoord;
in vec3 v_tangent;
in vec3 v_binormal;
in vec3 v_normal;
in vec3 v_surfaceToLight;
in vec3 v_surfaceToView;

out vec4 fragColor;

vec4 lit(float l, float h, float m) {
	return vec4(1.0, max(l, 0.0), (l > 0.0) ? pow(max(0.0, h), m) : 0.0, 1.0);
}

void main() {
	vec4 diffuseColor = texture(diffuse, v_texCoord);
	vec4 normalSpec = vec4(0.0);
	vec3 normal = normalize(v_normal);
	if (uNormal > 0.5) {
		normalSpec = texture(normalMap, v_texCoord);
		vec3 tangentNormal = normalSpec.xyz - vec3(0.5);
		if (uNormalBias > 0.5) {
			tangentNormal = normalize(tangentNormal + vec3(0.0, 0.0, 2.0));
		}
		mat3 tangentToWorld = mat3(v_tangent, v_binormal, v_normal);
		normal = normalize(tangentToWorld * tangentNormal);
	}
	vec3 surfaceToLight = normalize(v_surfaceToLight);
	vec3 surfaceToView = normalize(v_surfaceToView);
	vec3 halfVector = normalize(surfaceToLight + surfaceToView);
	vec4 litR = lit(dot(normal, surfaceToLight), dot(normal, halfVector), shininess);
	vec4 litColor = vec4((lightColor * (diffuseColor * litR.y + diffuseColor * ambient +
		specular * litR.z * specularFactor * normalSpec.a)).rgb, diffuseColor.a);
	vec4 outColor = litColor;
	if (uReflect > 0.5) {
		vec4 reflection = texture(reflectionMap, v_texCoord);
		vec4 skyColor = texture(skybox, -reflect(surfaceToView, normal));
		outColor = vec4(mix(skyColor, litColor, 1.0 - reflection.r).rgb, diffuseColor.a);
	}
	if (uFog > 0.5) {
		float fog = clamp(pow(v_position.z / v_position.w, fogPower) * fogMult - fogOffset, 0.0, 1.0);
		outColor = mix(outColor, vec4(fogColor.rgb, diffuseColor.a), fog);
	}
	fragColor = outColor;
}
