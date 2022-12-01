static const string shader_pbr_lighting_funcs = R"(

#ifndef FUNCTIONS_LIGHTING
#define FUNCTIONS_LIGHTING

//#define MIN_N_DOT_V 1e-4

//---------------------------------------------------
float clampNoV(in float NoV) {
	// Neubelt and Pettineo 2013, "Crafting a Next-gen Material Pipeline for The Order: 1886"
	return max(NoV, FLT_EPS);
}

//---------------------------------------------------
// https://github.com/google/filament/blob/main/shaders/src/brdf.fs
// Bruce Walter et al. 2007. Microfacet Models for Refraction through Rough Surfaces.
float D_GGX(float NoH, float roughness) {
	float a = NoH * roughness;
	float k = roughness / (1.0 - NoH * NoH + a * a);
	return saturateMediump(k * k * ONE_OVER_PI);//(1.0 / PI);
}

// Bruce Walter et al. 2007. Microfacet Models for Refraction through Rough Surfaces.
float D_GGX(float roughness, float NoH, const vec3 n, const vec3 h) {
	vec3 NxH = cross(n, h);
	float a = NoH * roughness;
	float k = roughness / (dot(NxH, NxH) + a * a);
	float d = k * k * ONE_OVER_PI;//(1.0 / PI);
	return saturateMediump(d);
}

//Eric Heitz. 2014. Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs.
//Journal of Computer Graphics Techniques, 3 (2).
float V_SmithGGXCorrelated(in float NoV, in float NoL, in float aroughness) {
	float a2 = aroughness * aroughness;
	float GGXV = NoL * sqrt((NoV - a2 * NoV) * NoV + a2);
	float GGXL = NoV * sqrt((NoL - a2 * NoL) * NoL + a2);
	float v = 0.5 / (GGXV + GGXL);
	return saturateMediump(v);
}

// Hammon 2017, "PBR Diffuse Lighting for GGX+Smith Microsurfaces"
float V_SmithGGXCorrelatedFast(in float NoV, in float NoL, in float roughness) {
	float v = 0.5 / mix(2.0 * NoL * NoV, NoL + NoV, roughness);
	return saturateMediump(v);
}

float V_Kelemen(float LoH) {
	// Kelemen 2001, "A Microfacet Based Coupled Specular-Matte BRDF Model with Importance Sampling"
	return saturateMediump(0.25 / (LoH * LoH));
}

float V_Neubelt(float NoV, float NoL) {
	// Neubelt and Pettineo 2013, "Crafting a Next-gen Material Pipeline for The Order: 1886"
	return saturateMediump(1.0 / (4.0 * (NoL + NoV - NoL * NoV)));
}

// Christophe Schlick. 1994. An Inexpensive BRDF Model for Physically-Based Rendering. Computer Graphics Forum, 13 (3), 233–246.
//vec3 F_Schlick(in float u, in vec3 f0, in float f90) {
//	return f0 + (vec3(f90) - f0) * pow(1.0 - u, 5.0);
//}
//
//vec3 F_Schlick(in float u, in vec3 f0) {
//	float f = pow(1.0 - u, 5.0);
//	return f + f0 * (1.0 - f);
//}

vec3 F_Schlick(const vec3 f0, float f90, float VoH) {
	// Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"
	return f0 + (f90 - f0) * pow5(1.0 - VoH);
}

vec3 F_Schlick(const vec3 f0, float VoH) {
	float f = pow(1.0 - VoH, 5.0);
	return f + f0 * (1.0 - f);
}

float F_Schlick(float f0, float f90, float VoH) {
	return f0 + (f90 - f0) * pow5(1.0 - VoH);
}

float Fd_Lambert() {
	return ONE_OVER_PI;//1.0 / PI;
}



// Brent Burley. 2012. Physically Based Shading at Disney.
// Physically Based Shading in Film and Game Production, ACM SIGGRAPH 2012 Courses.
float Fd_Burley(float roughness, float NoV, float NoL, float LoH) {
	// Burley 2012, "Physically-Based Shading at Disney"
	float f90 = 0.5 + 2.0 * roughness * LoH * LoH;
	float lightScatter = F_Schlick(1.0, f90, NoL);
	float viewScatter  = F_Schlick(1.0, f90, NoV);
	return lightScatter * viewScatter * ONE_OVER_PI;
}


//https://www.unrealengine.com/en-US/blog/physically-based-shading-on-mobile
vec3 EnvBRDFApprox( in vec3 SpecularColor, in float Roughness, in float NoV ) {
	const vec4 c0 = vec4( -1.0, -0.0275, -0.572, 0.022 );
	const vec4 c1 = vec4( 1.0, 0.0425, 1.04, -0.04 );
	vec4 r = Roughness * c0 + c1;
	float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
	vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;
	return SpecularColor * AB.x + AB.y;
}
//https://www.unrealengine.com/en-US/blog/physically-based-shading-on-mobile
vec2 EnvBRDFApproxDFG( in float Roughness, in float NoV ) {
	const vec4 c0 = vec4( -1.0, -0.0275, -0.572, 0.022 );
	const vec4 c1 = vec4( 1.0, 0.0425, 1.04, -0.04 );
	vec4 r = Roughness * c0 + c1;
	float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
	vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;
	return vec2(AB.x, AB.y);
}

// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}



// ----------------------------------------------------------------------------
// https://google.github.io/filament/Filament.md.html#toc9.5
float computeSpecularAO(float NoV, float ao, float roughness) {
#ifdef PBR_QUALITY_LEVEL_HIGH
	return clamp(pow(NoV + ao, exp2(-16.0 * roughness - 1.0)) - 1.0 + ao, 0.0, 1.0);
#else
	return 1.0;
#endif
}

#endif

)";
