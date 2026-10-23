#version 150
// Ported from the WebGL Aquarium seaweedFragmentShader.
// Copyright 2009, Google Inc. BSD-3-Clause.

uniform vec4 lightColor;
uniform vec4 ambient;
uniform vec4 specular;
uniform float shininess;
uniform float specularFactor;
uniform float uFog;
uniform float fogPower;
uniform float fogMult;
uniform float fogOffset;
uniform vec4 fogColor;
uniform sampler2D diffuse;

in vec4 v_position;
in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_surfaceToLight;
in vec3 v_surfaceToView;

out vec4 fragColor;

vec4 lit(float l, float h, float m) {
	return vec4(1.0, max(l, 0.0), (l > 0.0) ? pow(max(0.0, h), m) : 0.0, 1.0);
}

void main() {
	vec4 diffuseColor = texture(diffuse, v_texCoord);
	if (diffuseColor.a < 0.3) {
		discard;
	}
	vec3 normal = normalize(v_normal);
	vec3 surfaceToLight = normalize(v_surfaceToLight);
	vec3 surfaceToView = normalize(v_surfaceToView);
	vec3 halfVector = normalize(surfaceToLight + surfaceToView);
	vec4 litR = lit(dot(normal, surfaceToLight), dot(normal, halfVector), shininess);
	vec4 outColor = vec4((lightColor * (diffuseColor * litR.y + diffuseColor * ambient +
		specular * litR.z * specularFactor)).rgb, diffuseColor.a);
	if (uFog > 0.5) {
		float fog = clamp(pow(v_position.z / v_position.w, fogPower) * fogMult - fogOffset, 0.0, 1.0);
		outColor = mix(outColor, vec4(fogColor.rgb, diffuseColor.a), fog);
	}
	fragColor = outColor;
}
