static const string shader_pbr_data = R"(

#ifndef STRUCT_PBR_DATA
#define STRUCT_PBR_DATA

//#pragma include "pbrMaterial.glsl"

struct PbrData {
	vec3 directDiffuse;// = vec3(0.0);
	vec3 indirectDiffuse;// = vec3(0.0);
	
	vec3 directSpecular;// = vec3(0.0);
	vec3 indirectSpecular;// = vec3(0.0);
	
	vec3 diffuse;// = vec3(0.0);
	float NoV;// = 1.0;
	vec3 normalWorld;// = vec3(0.0,1.0,0.0);
	vec3 normalWorldGeometry;// = vec3(0.0,1.0,0.0);
	vec3 viewDirectionWorld; // normalize(uCameraPos - v_worldPosition);
	vec3 reflectionWorld;// = vec3( 0.0, 1.0, 0.0 );
	vec3 positionWorld;
	
	float iblLuminance;// = 1.0;
	
	vec3 f0;// = vec3(0.04);
	float f90;// = vec3(1.0);
};

void setupPbrData( inout PbrData adata, in Material amat, in vec3 aPosWorld, in vec3 aCamPos ) {
	vec3 N = amat.worldNormal;//normalize(v_worldNormal);
	vec3 V = normalize(aCamPos - aPosWorld);
	vec3 R = reflect(-V, N);
	
	adata.positionWorld = aPosWorld;
	adata.normalWorldGeometry = amat.normalWorldGeometry;
	
	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
	//	vec3 F0 = vec3(0.04);
	vec3 f0 = mix(amat.f0, amat.albedo.rgb, amat.metallic);
	//	f0 = 0.16 * mat.reflectance.rgb * mat.reflectance.rgb * (1.0 - mat.metallic) + mat.albedo.rgb * mat.metallic;
	f0 = 0.16 * amat.reflectance * amat.reflectance * (1.0 - amat.metallic) + amat.albedo.rgb * amat.metallic;
	
	adata.normalWorld = N;
	adata.reflectionWorld = R;
	adata.viewDirectionWorld = V;
	adata.NoV = max(dot(N, V), 0.0001);
	//	pbrData.f0 = mix(0.04, mat.albedo, mat.metallic);
	adata.f0 = f0;
#ifdef PBR_QUALITY_LEVEL_HIGH
	//	pbrData.f90 = vec3(clamp(dot(pbrData.f0.g, 50.0 * 0.33), 0.0, 1.0));
	adata.f90 = saturate(dot(f0, vec3(50.0 * 0.33)));
#else
	adata.f90 = 1.0;
#endif
	// cheap luminance approximation
	//	float f90 = clamp(50.0 * f0.g, 0.0, 1.0);
	adata.diffuse = amat.albedo.rgb * (1.0-amat.metallic);
	adata.iblLuminance = amat.iblExposure; // convert to luminance?
	
	adata.directDiffuse = vec3(0.0);
	adata.indirectDiffuse = vec3(0.0);
	
	adata.directSpecular = vec3(0.0);
	adata.indirectSpecular = vec3(0.0);
}

#endif

#ifndef STRUCT_LIGHT_DATA
#define STRUCT_LIGHT_DATA

struct PbrLightData {
	float enabled;
	//	vec4 ambient;
	// 0 = pointlight 1 = directionlight, 2 = spotlight, 3 = area light
	float type;
	// where are we in world space
	vec4 position;
	// light color, intensity is stored in .w
	vec4 diffuse;
	float radius;// = 0.0;
				 //	vec4 specular; // what kinda specular stuff we got going on?
				 // attenuation
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	//	// only for spot
	float spotCutoff;
	float spotCosCutoff;
	float spotExponent;
	//	// spot and area
	vec3 spotDirection;
	//	// only for directional
	//	vec3 halfVector;
	//	// only for area
	float width;
	float height;
	vec3 right;
	vec3 up;
};

uniform PbrLightData lights[MAX_LIGHTS];
#endif

)";
