static const string shader_pbr_lighting = R"(

#ifndef FUNCTIONS_PBR_LIGHT_TYPES
#define FUNCTIONS_PBR_LIGHT_TYPES

// https://google.github.io/filament/Filament.md.html#lighting/directlighting/directionallights
vec3 SurfaceSpecular(in PbrData adata, const vec3 h, float NoL, float NoH, float LoH, in float aroughness) {
	
	// perceptually linear roughness to roughness (see parameterization)
	float roughness = aroughness * aroughness;
	
	//	float D_GGX(float roughness, float NoH, const vec3 n, const vec3 h)
	float D = D_GGX( roughness, NoH, adata.normalWorld, h );
	
#ifdef PBR_QUALITY_LEVEL_HIGH
	//	float f90 = saturate(dot(f0, vec3(50.0 * 0.33)));
	vec3  F = F_Schlick(adata.f0, adata.f90, LoH);
	float V = V_SmithGGXCorrelated(adata.NoV, NoL, roughness);
#else
	vec3  F = F_Schlick(adata.f0, LoH);
	float V = V_SmithGGXCorrelatedFast(adata.NoV, NoL, roughness);
#endif
	
	// specular BRDF
	vec3 Fr = (D * V) * F;
	
	vec3 energyCompensation = 1.0 + adata.f0 * (1.0 / Fr.y - 1.0);
	// Scale the specular lobe to account for multiscattering
	//	Fr *= energyCompensation;
	
	return Fr;
}

float SurfaceDiffuse(float roughness, float NoV, float NoL, float LoH) {
#ifdef PBR_QUALITY_LEVEL_LOW
	// lambertian diffuse BRDF
	return Fd_Lambert();
#else
	// TODO: oren nayar?
	//	vec3 Fd = adata.diffuse * Fd_Lambert();
	return Fd_Burley(roughness, NoV, NoL, LoH);
#endif
}

// https://google.github.io/filament/Filament.md.html#materialsystem/clearcoatmodel
#if defined(HAS_CLEAR_COAT)
//vec2 SurfaceClearCoat(in PbrData adata, in Material amat, const vec3 h, float NoH, float LoH ) {
vec2 SurfaceClearCoat(in PbrData adata, in Material amat, float NoH, float LoH ) {
//	#if defined(HAS_TEX_CLEAR_COAT_NORMAL)
	// If the material has a normal map, we want to use the geometric normal
	// instead to avoid applying the normal map details to the clear coat layer
	//float clearCoatNoH = saturate(dot(adata.normalWorldGeometry, h));
//	#else
//	float clearCoatNoH = NoH;
//	#endif
	float D = D_GGX(NoH,amat.clearCoatRoughness*amat.clearCoatRoughness);
	float V = V_Kelemen(LoH);
	float F = F_Schlick(0.04, 1.0, LoH) * amat.clearCoat;//amat.clearCoat; // fix IOR to 1.5
	
	return vec2((D * V) * F, F);
}
#endif


float GetDistanceAttenuation( float distSq, float aLightRad) {
	float rad = max( FLT_EPS, aLightRad);
	float att = saturate(1.0 - distSq / (rad*rad));
	return att * att;
}

void calcLight(in int aLightIndex, inout PbrData adata, in Material amat ) {
	
	PbrLightData light = lights[aLightIndex];
	
	vec3 L = normalize(light.position.xyz);
	//	vec3 h = normalize(aV + aL);
	vec3 halfV = normalize( adata.viewDirectionWorld + L );
	
	//	float NoV = adata.NoV;//abs(dot(aN, aV)) + FLT_EPS;
	float NoL = saturate(dot(adata.normalWorld, L));
	float NoH = saturate(dot(adata.normalWorld, halfV));
	float LoH = saturate(dot(L, halfV));
	//
	//	vec3 SurfaceSpecular(in PbrData adata, const vec3 h, float NoL, float NoH, float LoH, in float aroughness)
	vec3 Fr = SurfaceSpecular( adata, halfV, NoL, NoH, LoH, amat.roughness );
	// diffuseColor = _mat.albedo.rgb * (vec3(1.0) - _mat.f0) * (1.0 - _mat.metallic);
	float sd = SurfaceDiffuse( amat.roughness, adata.NoV, NoL, LoH );
	// PbrData.diffuse is multiplied by (1.0-metallic).
//	vec3 Fd = adata.diffuse * (vec3(1.0)-adata.f0) * sd;
	vec3 Fd = adata.diffuse * sd;
	//	vec3 Fd = adata.diffuse * (vec3(1.0)-adata.f0) * Fd_Burley(aroughness, adata.NoV, NoL, LoH);
	//	fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
	//	vec3 kS = fresnelSchlickRoughness(NoH, vec3(0.04), 0.01 );
	//	vec3 kD = 1.0-kS;
	//	Fd = (adata.diffuse * Fd_Lambert()) * kD;
	
	//	vec3 Fd = adata.diffuse * sd;
	

	
	float attenuation = 0.0;
	float shadow = 0.0;
	
	if(light.type<0.5){
		// point light
		vec3 posToLight = adata.positionWorld - light.position.xyz;
		
		if( light.radius > 0.0 ) {
			float distanceSquare = dot(posToLight, posToLight);
			attenuation = GetDistanceAttenuation( distanceSquare, light.radius );
		} else {
			// from phong.frag
			float d = length(posToLight);
			attenuation = saturate(1.0 / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d));
		}
		//		attenuation = saturate(1.0 / (alight.constantAttenuation + alight.linearAttenuation * d + alight.quadraticAttenuation * d * d));
		//		attenuation = 1.0;
		attenuation *= NoL;
		
#ifdef HAS_SHADOWS
		if( shadows[aLightIndex].enabled > 0.5 ) {
			shadow = PointLightShadow(shadows[aLightIndex], adata.positionWorld, adata.normalWorld );
			shadow *= shadows[aLightIndex].strength;
		}
#endif
		
		//		attenuation = 1.0;
		//pointLight(lights[i], transformedNormal, v_eyePosition, 1.0-shadow, ambient, diffuse, specular);
	}else if(light.type<1.5){
		// directional light
		// calcDirectionalLight( inout PbrData adata, in PbrLightData alight, float aroughness, float ao, float ashadow )
		//calcDirectionalLight( pbrData, lights[i], mat.roughness, mat.ao, shadow );
		//directionalLight(lights[i], transformedNormal, 1.0-shadow, ambient, diffuse, specular);
		attenuation = NoL;
#ifdef HAS_SHADOWS
		if( shadows[aLightIndex].enabled > 0.5 ) {
			shadow = DirectionalShadow(shadows[aLightIndex], adata.positionWorld, adata.normalWorld );
			shadow *= shadows[aLightIndex].strength;
		}
#endif
	}else if(light.type<2.5){
		// spot light
		// Compute vector from surface to light position
		vec3 posToLight = light.position.xyz - adata.positionWorld;
		//VP = light.position.xyz - ecPosition3;
		float spotEffect = dot(light.spotDirection, -normalize(posToLight));
		
		if (spotEffect > light.spotCosCutoff) {
			
			float spotCut = radians(clamp(light.spotCutoff, 0.0, 89.9));
			float spotInner = clamp(spotCut - ((light.spotExponent/128.0) * HALF_PI), 0.0, spotCut-0.001);
			
			attenuation = smoothstep(spotCut, spotInner, acos(spotEffect));
			if( light.radius > 0.0 ) {
				float distanceSquare = dot(posToLight, posToLight);
				attenuation *= GetDistanceAttenuation( distanceSquare, light.radius );
			} else {
				// from phong.frag
				float d = length(posToLight);
				attenuation *= spotEffect / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);
			}
			attenuation = saturate(NoL * attenuation);
			
#ifdef HAS_SHADOWS
			if( shadows[aLightIndex].enabled > 0.5 ) {
				shadow = SpotShadow( shadows[aLightIndex], adata.positionWorld, adata.normalWorld );
				//				shadow *= attenuation;
				shadow *= shadows[aLightIndex].strength;
			}
#endif
			
		} else {
			attenuation = 0.0;
		}
		//spotLight(lights[i], transformedNormal, v_eyePosition, 1.0-shadow, ambient, diffuse, specular);
	}else{
		//areaLight(lights[i], transformedNormal, v_eyePosition, 1.0-shadow, ambient, diffuse, specular);
		// TODO: AREA LIGHTS!!
	}
	
	vec3 radiance = light.diffuse.rgb * ( light.diffuse.w * attenuation * amat.ao * (1.0-shadow));
//	Fd *= radiance;
//	Fr *= radiance;
	
	vec3 color = Fd + Fr;
#if defined(HAS_CLEAR_COAT)
	if (amat.clearCoat > 0.0) {
		
		float clearNoH = NoH;
		
		#if defined(HAS_TEX_CLEARCOAT_NORMAL)
		//float clearCoatNoV = clampNoV(dot(adata.clearcoatNormal, adata.viewDirectionWorld));
//		// the noh needs to recalc because it is different than the geometry
		clearNoH = saturate(dot(adata.clearcoatNormal, halfV));
		#else
		//float clearCoatNoV = clampNoV(dot(adata.normalWorldGeometry, adata.viewDirectionWorld));
		clearNoH = saturate(dot(adata.normalWorldGeometry, halfV));
		#endif
		
//		float attenuation = 1.0 - Fc;
//		Fd *= attenuation;
//		Fr *= attenuation;
//		
//		// TODO: Should we apply specularAO to the attenuation as well?
//		//		float specularAO = specularAO(clearCoatNoV, diffuseAO, adata.clearCoatRoughness, cache);
//		// computeSpecularAO(adata.NoV, ao, aroughness);
//		float specAO = computeSpecularAO(clearCoatNoV, amat.ao, amat.clearCoatRoughness );
//		Fr += getIndirectPrefilteredReflection(clearCoatR, amat.clearCoatRoughness) * (specAO * Fc);
		
		//float specAO = computeSpecularAO(clearCoatNoV, amat.ao, amat.clearCoatRoughness );
		//Fr += getIndirectPrefilteredReflection(clearCoatR, amat.clearCoatRoughness) * (specAO * Fc);
		
		//	SurfaceClearCoat(in PbrData adata, in Material amat, const vec3 h, float NoH, float LoH )
//		vec2 clearCoatFactor = SurfaceClearCoat(adata, amat, halfV, clearNoH, LoH );
		vec2 clearCoatFactor = SurfaceClearCoat(adata, amat, clearNoH, LoH );
		float Fcc = clearCoatFactor.y;
		float clearSpec = clearCoatFactor.x;
		
		// // account for energy loss in the base layer
//		color *= ((Fd + Fr * (1.0 - Fcc)) * (1.0 - Fcc) + clearSpec);
		float cattenuation = 1.0 - Fcc;
//		Fd *= cattenuation;
//		Fr *= cattenuation;
//		Fr += clearSpec * cattenuation * cattenuation;
//		Fr = Fr * cattenuation * cattenuation;
		Fd = ((Fd * cattenuation) + clearSpec);
		//Fr = saturate((Fr * cattenuation * cattenuation) + clearSpec);// * (specAO*Fcc);
		Fr += ((Fd + (Fr * cattenuation)*cattenuation) + (clearSpec));// * (specAO*Fcc);
//		Fr = saturate(Fr);
	//	vec3 color = Fd + Fr;
//		color *= cattenuation;
//		color += clearSpec;
	//	Fr = vec3(1.0, 0.0, 0.0);
	//	Fd = vec3(1.0, 0.0, 0.0);
	}
#endif
	
	Fd *= radiance;
	Fr *= radiance;
	
	adata.directDiffuse += Fd;// * radiance;
	adata.directSpecular += Fr;// * radiance;
	
	
	//vec3 radiance = light.diffuse.rgb * ( light.diffuse.w * attenuation * amat.ao * (1.0-shadow));
//	adata.directDiffuse += color * radiance;
	//adata.directDiffuse += Fd * radiance;
	//adata.directSpecular += Fr * radiance;
}



#endif


)";
