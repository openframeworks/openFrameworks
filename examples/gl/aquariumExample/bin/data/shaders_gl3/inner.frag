#version 150
// Ported from the WebGL Aquarium innerRefractionMapFragmentShader.
// Copyright 2009, Google Inc. BSD-3-Clause.

uniform float uNormal;
uniform float uFog;
uniform float refractionFudge;
uniform float eta;
uniform float tankColorFudge;
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
in vec3 v_surfaceToView;

out vec4 fragColor;

void main() {
	vec4 diffuseColor = texture(diffuse, v_texCoord) + vec4(tankColorFudge, tankColorFudge, tankColorFudge, 1.0);
	vec4 refraction = texture(reflectionMap, v_texCoord);
	vec3 normal = normalize(v_normal);
	if (uNormal > 0.5) {
		vec4 normalSpec = texture(normalMap, v_texCoord);
		vec3 tangentNormal = normalSpec.xyz - vec3(0.5);
		tangentNormal = normalize(tangentNormal + vec3(0.0, 0.0, refractionFudge));
		mat3 tangentToWorld = mat3(v_tangent, v_binormal, v_normal);
		normal = normalize(tangentToWorld * tangentNormal);
	}
	vec3 surfaceToView = normalize(v_surfaceToView);
	vec3 refractionVec = refract(surfaceToView, normal, eta);
	vec4 skyColor = texture(skybox, refractionVec);
	vec4 outColor = vec4(mix(skyColor * diffuseColor, diffuseColor, refraction.r).rgb, diffuseColor.a);
	if (uFog > 0.5) {
		float fog = clamp(pow(v_position.z / v_position.w, fogPower) * fogMult - fogOffset, 0.0, 1.0);
		outColor = mix(outColor, vec4(fogColor.rgb, diffuseColor.a), fog);
	}
	fragColor = outColor;
}
