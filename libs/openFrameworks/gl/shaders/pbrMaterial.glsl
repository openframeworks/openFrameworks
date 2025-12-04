static const string shader_pbr_material = R"(

//#ifndef SAMPLE_TEXTURE
//#define SAMPLE_TEXTURE(TEX,UV) texture(TEX, UV )
//#endif

#ifndef MATERIAL_STRUCT
#define MATERIAL_STRUCT
struct Material {
	// material properties
	vec4 albedo;// = vec4( vec3(0.5), 1.0);
	vec4 emissive;// = vec4(0.0);
	float ao;// = 1.0;
	float roughness;// = 0.3;
	float metallic;// = 0.0;
	float reflectance;// = 0.5;
	float clearCoat; // 0
	float clearCoatRoughness;// 0.1
	
	vec3 f0;// = vec3(0.04);
	vec3 f90;// = vec3(1.0);
	
	vec3 worldNormal;// = (1.0, 0.0, 0.0);
	vec3 normalWorldGeometry;
	vec3 clearcoatNormal;
	
	vec2 uv;
	float iblExposure;
};
#endif

#ifndef FUNCTIONS_MATERIALS
#define FUNCTIONS_MATERIALS

//#pragma include "shaderUtils.glsl"

// material parameters
uniform vec4 mat_diffuse;
uniform vec4 mat_emissive;
uniform float mat_roughness;
uniform float mat_metallic;
uniform float mat_reflectance;
uniform float mat_ibl_exposure;
#if defined(HAS_CLEAR_COAT)
uniform vec2 mat_clearcoat;
#endif
#if defined(HAS_TEX_NORMAL) || defined(HAS_TEX_DISPLACEMENT)
uniform float mat_normal_mix;
#endif
#if defined(HAS_TEX_DISPLACEMENT)
uniform float mat_displacement_strength;
uniform float mat_displacement_normals_strength;
#endif

#if HAS_TEXTURE
uniform SAMPLER tex0;
#endif

vec4 getMaterialAlbedo(in vec2 aUv) {
	vec4 albedo = mat_diffuse;
	#if defined(HAS_TEXTURE)
		#if HAS_TEXTURE
			albedo = TEXTURE(tex0, aUv);
		#endif
	#endif
	
	#if defined(HAS_COLOR)
		#if HAS_COLOR
			albedo *= v_color;
		#endif
	#endif
	return gamma2Linear(albedo);
}

// apply emmisive texture
//#if defined(HAS_TEX_EMISSIVE)
//uniform SAMPLER tex_emissive;
//#endif

vec4 getMaterialEmissive(in vec2 aUv) {
	vec4 emission = mat_emissive;
#if defined(HAS_TEX_EMISSIVE)
	emission = TEXTURE(tex_emissive, aUv);
#endif
	return gamma2Linear(emission);
}

//#if defined(HAS_TEX_OCCLUSION)
//uniform SAMPLER tex_occlusion;
//#endif

//#if defined(HAS_TEX_AO_ROUGHNESS_METALLIC)
//uniform SAMPLER tex_ao_roughness_metallic;
//#endif

float getMaterialAO(in vec2 aUv) {
	float ao = 1.0;
#if defined(HAS_TEX_OCCLUSION)
	ao = TEXTURE(tex_occlusion, aUv).r;
#elif defined(HAS_TEX_AO_ROUGHNESS_METALLIC)
	ao = TEXTURE(tex_ao_roughness_metallic, aUv).r;
#endif
	return ao;
}

//#if defined(HAS_TEX_ROUGHNESS_METALLIC)
//uniform SAMPLER tex_roughness_metallic;
//#endif


//#if defined(HAS_TEX_ROUGHNESS)
//uniform SAMPLER tex_roughness;
//#endif

float getMaterialRoughness(in vec2 aUv) {
	float roughness = mat_roughness;
#if defined(HAS_TEX_AO_ROUGHNESS_METALLIC)
	roughness = TEXTURE(tex_ao_roughness_metallic, aUv).g;
#elif defined(HAS_TEX_ROUGHNESS_METALLIC)
	roughness = TEXTURE(tex_roughness_metallic, aUv).g;
#elif defined(HAS_TEX_ROUGHNESS)
	roughness = TEXTURE(tex_roughness, aUv).g;
#endif
	return max(roughness, 0.001);
}


//#if defined(HAS_TEX_METALLIC)
//uniform SAMPLER tex_metallic;
//#endif

float getMaterialMetallic(in vec2 aUv) {
	float metalness = mat_metallic;
	#if defined(HAS_TEX_AO_ROUGHNESS_METALLIC)
	metalness = TEXTURE(tex_ao_roughness_metallic, aUv).b;
	#elif defined(HAS_TEX_ROUGHNESS_METALLIC)
	metalness = TEXTURE(tex_roughness_metallic, aUv).b;
	#elif defined(HAS_TEX_METALLIC)
	metalness = TEXTURE(tex_metallic, aUv).b;
	#endif
	return metalness;
}

vec3 getAoRoughnessMetallic(in vec2 aUv) {
	vec3 arm = vec3(1.0, mat_roughness, mat_metallic );
	#if defined(HAS_TEX_AO_ROUGHNESS_METALLIC)
		arm.rgb = TEXTURE(tex_ao_roughness_metallic, aUv).rgb;
	#else
		#if defined(HAS_TEX_OCCLUSION)
			arm.r = TEXTURE(tex_occlusion, aUv).r;
		#endif
		
		#if defined(HAS_TEX_ROUGHNESS_METALLIC)
			arm.gb = TEXTURE(tex_roughness_metallic, aUv).gb;
		#else
			#if defined(HAS_TEX_ROUGHNESS)
				arm.g = TEXTURE(tex_roughness, aUv).g;
			#endif
			#if defined(HAS_TEX_METALLIC)
				arm.b = TEXTURE(tex_metallic, aUv).b;
			#endif
		#endif
	#endif
	arm.g = max(arm.g, 0.001);
	return arm;
}

//#if defined(HAS_TEX_NORMAL)
//uniform SAMPLER tex_normal;
//#endif

vec3 getMaterialNormal(in vec2 aUv, in vec3 aNormal, in vec3 aPos ) {
	vec3 tnormal = v_worldNormal;
	#if defined(HAS_TEX_NORMAL)
	tnormal = PerturbNormal(TEXTURE(tex_normal, aUv).rgb, aNormal, aPos, aUv);
	tnormal = mix(v_worldNormal, tnormal, mat_normal_mix );
	#endif
	return normalize(tnormal);
}

float getMaterialReflectance(in vec2 aUv) {
	//TODO: add a reflectance map
	return mat_reflectance;
}

//#if defined(HAS_TEX_CLEARCOAT_INTENSITY)
//uniform SAMPLER tex_clearcoat_intensity;
//#endif

//#if defined(HAS_TEX_CLEARCOAT_ROUGHNESS)
//uniform SAMPLER tex_clearcoat_roughness;
//#endif

//#if defined(HAS_TEX_CLEARCOAT_INTENSITY_ROUGHNESS)
//uniform SAMPLER tex_clearcoat_intensity_roughness;
//#endif



// https://github.com/KhronosGroup/glTF/tree/main/extensions/2.0/Khronos/KHR_materials_clearcoat
vec2 getMaterialClearcoat(in vec2 aUv) {
	// intensity, roughness
	vec2 clearCoat = vec2(0.0, 0.1);
	#if defined(HAS_CLEAR_COAT)
		clearCoat = mat_clearcoat;
		#if defined(HAS_TEX_CLEARCOAT_INTENSITY_ROUGHNESS)
			vec3 tc = TEXTURE(tex_clearcoat_intensity_roughness, aUv).rgb;
			clearCoat.x *= tc.r;
			clearCoat.y *= tc.g;
		#else
			#if defined(HAS_TEX_CLEARCOAT_INTENSITY)
				clearCoat.x *= TEXTURE(tex_clearcoat_intensity, aUv).r;
			#endif
			#if defined(HAS_TEX_CLEARCOAT_ROUGHNESS)
				clearCoat.y *= TEXTURE(tex_clearcoat_roughness, aUv).g;
			#endif
		#endif
	#endif
	return clearCoat;
}

//#if defined(HAS_TEX_CLEARCOAT_NORMAL)
//uniform SAMPLER tex_clearcoat_normal;
//#endif

vec3 getMaterialClearCoatNormal(in vec2 aUv, in vec3 aNormal) {
	vec3 ccn = aNormal;
	#if defined(HAS_CLEAR_COAT)
		#if defined(HAS_TEX_CLEARCOAT_NORMAL)
			ccn = PerturbNormal(TEXTURE(tex_clearcoat_normal, aUv).rgb, aNormal, aPos, aUv);
		#endif
	#endif
	return ccn;
}

void setupMaterial( inout Material amat ) {
	amat.albedo = getMaterialAlbedo(amat.uv);
	amat.emissive = getMaterialEmissive(amat.uv);
//	amat.ao = getMaterialAO();
//	amat.roughness = getMaterialRoughness();
//	amat.metallic = getMaterialMetallic();
	vec3 arm = getAoRoughnessMetallic(amat.uv);
	amat.ao = arm.r;
	amat.roughness = arm.g;
	amat.metallic = arm.b;
	amat.reflectance = getMaterialReflectance(amat.uv);
#if defined(HAS_CLEAR_COAT)
	vec2 cc = getMaterialClearcoat(amat.uv);
	amat.clearCoat = cc.x; // 0
	amat.clearCoatRoughness = cc.y;// 0.1
#else
	amat.clearCoat = 0.0;
	amat.clearCoatRoughness = 0.1;
#endif
	amat.clearCoatRoughness = saturate( amat.clearCoatRoughness + 0.01 );
	
	// set some minimums, otherwise things can render black ....
	amat.metallic = saturate( amat.metallic + FLT_EPS);
	amat.roughness = saturate( amat.roughness + 0.01 );
	// reflectance only applies to dielectrics, ie, non metals
	amat.reflectance = saturate( amat.reflectance + FLT_EPS);
	
	amat.iblExposure = mat_ibl_exposure;
	
	amat.f0 = vec3(0.04);
	amat.f90 = vec3(1.0);
	amat.worldNormal = getMaterialNormal(amat.uv, v_worldNormal, -v_worldPosition );
	amat.normalWorldGeometry = v_worldNormal;
	#if defined(HAS_CLEAR_COAT)
	amat.clearcoatNormal = getMaterialClearCoatNormal(amat.uv, amat.normalWorldGeometry);
	#else
	amat.clearcoatNormal = amat.normalWorldGeometry;
	#endif
	
	#if defined(HAS_TEX_DISPLACEMENT)
	float dstrength = mat_displacement_normals_strength * mat_displacement_strength;
	vec3 nnormal = perturbNormalArb( -v_worldPosition, v_worldNormal, dHdxy_fwd(tex_displacement, amat.uv, dstrength), -1.0 );
	amat.worldNormal = mix(amat.worldNormal, nnormal, mat_normal_mix );
	#endif
	
}

#endif

)";
