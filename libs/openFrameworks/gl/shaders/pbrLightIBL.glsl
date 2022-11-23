static const string shader_pbr_lighting_ibl = R"(

#ifndef FUNCTIONS_ENV_LIGHT
#define FUNCTIONS_ENV_LIGHT

//#pragma include "pbrLightingFuncs.glsl"
//#pragma include "pbrData.glsl"

#ifndef ENV_MAP_MAX_MIPS
#define ENV_MAP_MAX_MIPS 5
#endif

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

//vec3 getEnvIrradiance(in vec3 aN) {
//	vec3 diffuse = vec3(0.0);
//	// TODO: implement spherical harmonics if tex_irradianceMap not present
//	#ifdef HAS_TEX_ENV_IRRADIANCE
//	diffuse = texture(tex_irradianceMap, aN).rgb;
//	#endif
//	return diffuse;
//}

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

//vec3 getEnvironmentIndirect(in vec3 aN, in float aNoV, in vec3 aR, in vec3 aF0, in vec3 aF90, in vec3 aDiffuseColor, in float aroughness in float ao ) {
void calcEnvironmentIndirect( inout PbrData adata, in Material amat ) {
	//	float NoV = max(dot(aN, aV), 0.0);
	// ambient lighting (we now use IBL as the ambient term)
	//	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), aF0, aroughness);
	//	vec3 F = fresnelSchlickRoughness(nDotV, aF0, aroughness);
	//	float f90 = 0.5 + 2.0 * roughness * LoH * LoH;
	//	vec3 f90 = 1.0;
	
	//	vec3 kS = F;
	//	vec3 kD = 1.0 - kS;
	//	kD *= 1.0 - ametallic;
	
	// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
	vec3 indirectSpecularRadiance = vec3(0.0);
#if defined(HAS_TEX_ENV_PRE_FILTER)
	//	indirectSpecularRadiance = textureLod(tex_prefilterEnvMap, adata.reflectionWorld, aroughness * (ENV_MAP_MAX_MIPS-1) ).rgb;
	indirectSpecularRadiance = getIndirectPrefilteredReflection( adata.reflectionWorld, amat.roughness *amat.roughness );
	indirectSpecularRadiance = indirectSpecularRadiance / (indirectSpecularRadiance+vec3(1.0));
	// horizon occlusion with falloff, should be computed for direct specular too
	float horizon = min(1.0 + dot(adata.reflectionWorld, adata.normalWorld), 1.0);
	indirectSpecularRadiance *= horizon * horizon;
#endif
	
	vec3 specularReflectance = vec3(0.0);
	vec2 brdf = vec2(0.0, 0.0);
#if defined(PBR_QUALITY_LEVEL_HIGH) && defined(HAS_TEX_ENV_BRDF_LUT)
	//	vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), aroughness)).rg;
	brdf = texture(tex_brdfLUT, vec2(adata.NoV, amat.roughness)).rg;
	specularReflectance = (adata.f0 * brdf.x + adata.f90 * brdf.y);
	// vec3 specularColor = f0 * brdf.x + f90 * brdf.y;
#else
	brdf = EnvBRDFApproxDFG( amat.roughness*amat.roughness, adata.NoV );
	specularReflectance = (adata.f0 * brdf.x + adata.f90 * brdf.y);
//	specularReflectance = mix(brdf.xxx, brdf.yyy, adata.f0);
	//	specularReflectance = EnvBRDFApprox( adata.f0, aroughness, adata.NoV );
	//return SpecularColor * AB.x + AB.y;
#endif
	
	vec3 Fr = specularReflectance * indirectSpecularRadiance;
	// float NoV, float ao, float roughness
	
	//	vec3 energyCompensation = 1.0 + f0 * (1.0 / dfg.y - 1.0);
	//vec3 energyCompensation = 1.0 + adata.f0 * (1.0 / (1.0-brdf.y) - 1.0);
	vec3 energyCompensation = 1.0 + adata.f0 * (1.0 / specularReflectance.y - 1.0);
	// Scale the specular lobe to account for multiscattering
	Fr *= energyCompensation;
	float specAO = computeSpecularAO(adata.NoV, amat.ao, amat.roughness);
	Fr *= specAO;
	
	
	vec3 Fd = adata.diffuse;//vec3(0.0);
	vec3 irradiance = vec3(0.0);
	// TODO: implement spherical harmonics if tex_irradianceMap not present
#ifdef HAS_TEX_ENV_IRRADIANCE
	irradiance = texture(tex_irradianceMap, adata.normalWorld).rgb;
	// HDR Tone mapping
	irradiance = irradiance / (irradiance + vec3(1.0));
	Fd *= irradiance;// * (1.0);
	//vec3 Fd = adata.diffuse * irradiance * (1.0 - E) * diffuseBRDF;
#endif
//	vec3 kS = fresnelSchlick(max(adata.NoV, 0.01), vec3(0.04));
//	vec3 kD = 1.0 - kS;
	//Fd *= kD;
	//Fd *= Fd_Lambert();
	// Fd_Burley(float roughness, float NoV, float NoL, float LoH)
	Fd *= amat.ao;
	Fd *= ((1.0-specularReflectance));
	//	Fd = vec3(1.0,0.0,0.0);
	
	evaluateClearCoatIBL(adata, amat, Fd, Fr);
	
	adata.indirectDiffuse += (Fd * adata.iblLuminance);//irradiance * adata.diffuse* Fd_Lambert() * (1.0-specularReflectance) * ao;
	adata.indirectSpecular += (Fr * adata.iblLuminance);
	//	adata.indirectSpecular += indirectSpecularRadiance;
	
	//return Fd + Fr;
}

#endif

)";
