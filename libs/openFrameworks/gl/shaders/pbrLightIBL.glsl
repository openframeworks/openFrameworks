static const string shader_pbr_lighting_ibl = R"(

#ifndef FUNCTIONS_ENV_LIGHT
#define FUNCTIONS_ENV_LIGHT

//#pragma include "pbrLightingFuncs.glsl"
//#pragma include "pbrData.glsl"

#ifndef ENV_MAP_MAX_MIPS
#define ENV_MAP_MAX_MIPS 5
#endif

//#ifdef HAS_CUBE_MAP
//#endif

// Image Based Lighting
#ifdef HAS_TEX_ENV_IRRADIANCE
uniform samplerCube tex_irradianceMap;
#endif

#ifdef HAS_TEX_ENV_PRE_FILTER
uniform samplerCube tex_prefilterEnvMap;
#endif

#ifdef HAS_TEX_ENV_BRDF_LUT
uniform sampler2D tex_brdfLUT;
#endif

uniform float uCubeMapEnabled;

vec3 getIndirectPrefilteredReflection( in vec3 aR, in float aroughness ) {
	vec3 indirectSpecularRadiance = vec3(0.0);
	//	float lod = pow(aroughness, 2.0) * (ENV_MAP_MAX_MIPS-1);
	
#if defined(HAS_TEX_ENV_PRE_FILTER)
	float lod = aroughness * (ENV_MAP_MAX_MIPS-1);
	//	indirectSpecularRadiance = textureLod(tex_prefilterEnvMap, aR, aroughness * (ENV_MAP_MAX_MIPS-1) ).rgb;
	vec3 ai = textureLod(tex_prefilterEnvMap, aR, floor(lod) ).rgb;
	vec3 ab = textureLod(tex_prefilterEnvMap, aR, clamp(ceil(lod), floor(lod), ENV_MAP_MAX_MIPS-1) ).rgb;
	indirectSpecularRadiance = mix(ai, ab, lod-floor(lod) );
#endif
	return indirectSpecularRadiance;
}

// https://google.github.io/filament/Filament.md.html#materialsystem/clearcoatmodel
void evaluateClearCoatIBL(const PbrData adata, const Material amat, inout vec3 Fd, inout vec3 Fr) {	
#if defined(HAS_CLEAR_COAT)
	if (amat.clearCoat > 0.0) {
//		#if defined(HAS_TEX_CLEAR_COAT_NORMAL)
		// We want to use the geometric normal for the clear coat layer
		#if defined(HAS_TEX_CLEARCOAT_NORMAL)
		// the noh needs to recalc because it is different than the geometry
		vec3 clearNormal = adata.clearcoatNormal;
		#else
		vec3 clearNormal = adata.normalWorldGeometry;
		#endif
		
		float clearCoatNoV = clampNoV(dot(clearNormal, adata.viewDirectionWorld));
		vec3 clearCoatR = reflect(-adata.viewDirectionWorld, clearNormal);
//		#else
//		float clearCoatNoV = adata.NoV;
//		vec3 clearCoatR = adata.reflectionWorld;
//		#endif
		// The clear coat layer assumes an IOR of 1.5 (4% reflectance)
		float Fc = F_Schlick(0.04, 1.0, clearCoatNoV) * amat.clearCoat;
		float attenuation = 1.0 - Fc;
		Fd *= attenuation;
		Fr *= attenuation;
		
		float horizon = min(1.0 + dot(clearCoatR, clearNormal), 1.0);
//		indirectSpecularRadiance *= horizon * horizon;
		
		// TODO: Should we apply specularAO to the attenuation as well?
//		float specularAO = specularAO(clearCoatNoV, diffuseAO, adata.clearCoatRoughness, cache);
		// computeSpecularAO(adata.NoV, ao, aroughness);
		float specAO = horizon * horizon * computeSpecularAO(clearCoatNoV, amat.ao, amat.clearCoatRoughness );
		Fr += getIndirectPrefilteredReflection(clearCoatR, amat.clearCoatRoughness*amat.clearCoatRoughness) * (specAO * Fc);
	}
#endif
}

// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// ----------------------------------------------------------------------------
vec3 fakeEnvLightingDiffuse(in PbrData adata, in Material amat ) {
	vec3 R = abs(adata.reflectionWorld);
	float cc = max( 0.0, abs( dot(R, vec3(0.0,0.5,0.5) )));
	return vec3(clamp(cc*0.25+0.6,0,1));
}

// ----------------------------------------------------------------------------
vec3 fakeEnvLightingSpecular(in PbrData adata, in Material amat ) {
	vec3 R = abs(adata.reflectionWorld);
	float mm = (amat.roughness * amat.roughness);
	float ss = (1.0-mm*0.95);
	ss *= ss;
	ss *= ss;
	ss = ss * 12.0;
	float cc = max( 0.0, abs( dot(R, vec3(0.0,0.5,0.5)*1.43 )));
	cc = pow(cc+0.005, ss+0.05);
	return vec3(clamp(cc,0,1));
}

void calcEnvironmentIndirect( inout PbrData adata, in Material amat ) {
#ifdef HAS_CUBE_MAP
	if( uCubeMapEnabled < 0.5 ) {
		return;
	}
#endif
	
	vec3 indirectSpecularRadiance = vec3(0.0);
	
	// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
#if defined(HAS_TEX_ENV_PRE_FILTER)
	indirectSpecularRadiance = getIndirectPrefilteredReflection( adata.reflectionWorld, amat.roughness );
	indirectSpecularRadiance = indirectSpecularRadiance / (indirectSpecularRadiance+vec3(1.0));
	// horizon occlusion with falloff, should be computed for direct specular too
	float horizon = min(1.0 + dot(adata.reflectionWorld, adata.normalWorld), 1.0);
	indirectSpecularRadiance *= horizon * horizon;
#else
	indirectSpecularRadiance = fakeEnvLightingSpecular(adata, amat);
#endif
	
	// adata.dfg and adata.energyCompensation are set in pbrData.glsl :: setupPbrData()
	vec3 Fr = indirectSpecularRadiance * adata.dfg * adata.energyCompensation;
	float specAO = computeSpecularAO(adata.NoV, amat.ao, amat.roughness);
	Fr *= specAO;
	
	vec3 Fd = adata.diffuse;// amat.albedo.rgb * (1.0-amat.metallic);//vec3(0.0);
	vec3 irradiance = vec3(0.0);
	// TODO: implement spherical harmonics if tex_irradianceMap not present
#ifdef HAS_TEX_ENV_IRRADIANCE
	irradiance = texture(tex_irradianceMap, adata.normalWorld).rgb;
	// HDR Tone mapping
	irradiance = irradiance / (irradiance + vec3(1.0));
	Fd *= irradiance;// * (1.0);
	//vec3 Fd = adata.diffuse * irradiance * (1.0 - E) * diffuseBRDF;
#else
	irradiance = fakeEnvLightingDiffuse(adata, amat);
	Fd *= irradiance;
#endif
	Fd *= amat.ao;
	Fd *= (1.0-adata.dfg);
	
	evaluateClearCoatIBL(adata, amat, Fd, Fr);
	
	adata.indirectDiffuse += (Fd * adata.iblLuminance);
	adata.indirectSpecular += (Fr * adata.iblLuminance);
}

#endif

)";
