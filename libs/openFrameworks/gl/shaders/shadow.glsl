static const string shadow_shader_include = R"(
struct shadowData {
	float enabled;
	vec3 lightWorldPos;
	vec3 lightUp;
	vec3 lightRight;
	float near;
	float far;
	float shadowType; // 0 = hard, 1 = pcf 2x2, 2 = pcf 9 samples, 3 = pcf 20 samples
	int texIndex;
	float strength;
	float bias;
	float normalBias;
	float sampleRadius;
	mat4 shadowMatrix;
};

#if defined(MAX_LIGHTS) && MAX_LIGHTS
uniform shadowData shadows[MAX_LIGHTS];
#endif

#ifndef FLT_EPS
#define FLT_EPS            0.0006103515625
#endif

#ifndef ONE_LESS_FLT_EPS
#define ONE_LESS_FLT_EPS            1.0-FLT_EPS
#endif

#ifdef SHADOWS_USE_CUBE_MAP_ARRAY
uniform samplerCubeArray uShadowCubeMap;
#else
uniform samplerCube uShadowCubeMap;
#endif

#ifdef SHADOWS_USE_TEXTURE_ARRAY
uniform sampler2DArrayShadow uShadowMapDirectional;
uniform sampler2DArrayShadow uShadowMapSpot;
uniform sampler2DArray uShadowMapArea;
#else
uniform sampler2DShadow uShadowMapDirectional;
uniform sampler2DShadow uShadowMapSpot;
uniform sampler2D uShadowMapArea;
#endif

mediump vec2 cardinalSampleDisk4_v2[4] = vec2[](vec2(-1, 0), vec2(1, 0), vec2(0, -1), vec2(0, 1));
mediump vec2 cornerSampleDisk4_v2[4] = vec2[](vec2(-1, -1), vec2(1, -1), vec2(1, 1), vec2(-1, 1));
mediump vec2 poissonDisk16_v2[16] = vec2[](
										 vec2( -0.94201624, -0.39906216 ),vec2( 0.94558609, -0.76890725 ),
										 vec2( -0.094184101, -0.92938870 ),vec2( 0.34495938, 0.29387760 ),
										 vec2( -0.91588581, 0.45771432 ),vec2( -0.81544232, -0.87912464 ),
										 vec2( -0.38277543, 0.27676845 ),vec2( 0.97484398, 0.75648379 ),
										 vec2( 0.44323325, -0.97511554 ),vec2( 0.53742981, -0.47373420 ),
										 vec2( -0.26496911, -0.41893023 ),vec2( 0.79197514, 0.19090188 ),
										 vec2( -0.24188840, 0.99706507 ),vec2( -0.81409955, 0.91437590 ),
										 vec2( 0.19984126, 0.78641367 ),vec2( 0.14383161, -0.14100790 ));

// vec 3 arrays of ofset direction for sampling
mediump vec3 cardinalSamplingDisk6_v3[6] = vec3[](vec3(0, 0, -1), vec3(0, 0, 1),vec3(-1, 0, 0), vec3(-1, 0, 0),vec3(0, -1, 0), vec3(0, 1, 0));
mediump vec3 cornerSamplingDisk8_v3[8] = vec3[](vec3(-1, -1, -1), vec3(1, -1, -1), vec3(1, 1, -1), vec3(-1, 1, -1),vec3(-1, -1, 1), vec3(1, -1, 1), vec3(1, 1, 1), vec3(-1, 1, 1));
// array of offset direction for sampling
mediump vec3 gridSamplingDisk20_v3[20] = vec3[](
 vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
 vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
 vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
 vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
 vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

const vec2 shadowCoordsMin = vec2(FLT_EPS);
const vec2 shadowCoordsMax = vec2(ONE_LESS_FLT_EPS);

#ifdef SHADOWS_USE_CUBE_MAP_ARRAY
void AddShadowAmountCubeMap( in samplerCubeArray aShadowMap, in float acurrentDepth, in vec3 st3, in int aTexIndex, inout float ashadow ) {
#else
void AddShadowAmountCubeMap( in samplerCube aShadowMap, in float acurrentDepth, in vec3 st3, in int aTexIndex, inout float ashadow ) {
#endif
#ifdef SHADOWS_USE_CUBE_MAP_ARRAY
	float closestDepth = texture(aShadowMap, vec4(st3, aTexIndex) ).r;
#else
	float closestDepth = texture(aShadowMap, st3 ).r;
#endif
	if(closestDepth < 1.0 && acurrentDepth > closestDepth) {
		ashadow += 1.0;
	}
}

#ifdef SHADOWS_USE_CUBE_MAP_ARRAY
float SampleShadowCube(in shadowData aShadowData, in samplerCubeArray aShadowMap, vec3 fragToLight, float acurrentDepth, float aclosestDepth, float offset, float abias ) {
#else
float SampleShadowCube(in shadowData aShadowData, in samplerCube aShadowMap, vec3 fragToLight, float acurrentDepth, float aclosestDepth, float offset, float abias ) {
#endif
	float shadow = 0.0;
	float currentDepth = acurrentDepth;
	float closestDepth = aclosestDepth;
	if(closestDepth < 1.0 && currentDepth - abias > closestDepth) {
		shadow = 1.f;
	}
	// adjust for bias here //
	currentDepth = currentDepth - abias;
	vec3 st3 = fragToLight;
	if( aShadowData.shadowType < 1.0 ) {
		// already set shadow above
	} else if( aShadowData.shadowType < 2.0 ) {
		for( int i = 0; i < 8; i++ ) {
			st3 = fragToLight + (cornerSamplingDisk8_v3[i] * offset);
			AddShadowAmountCubeMap( aShadowMap, currentDepth, st3, aShadowData.texIndex, shadow );
		}
		// dividing by 9 because shadow was set initially and added a grid of 8
		shadow /= 9.0;
	} else if( aShadowData.shadowType < 3.0 ) {
		int samples = 8;
		for(int i = 0; i < samples; ++i) {
			st3 = fragToLight + (cornerSamplingDisk8_v3[i] * offset);
			AddShadowAmountCubeMap( aShadowMap, currentDepth, st3, aShadowData.texIndex, shadow );
		}
		if( shadow > 0.0 ) {
			int samples = 6;
			for(int i = 0; i < samples; ++i) {
				st3 = fragToLight + (cardinalSamplingDisk6_v3[i] * offset);
				AddShadowAmountCubeMap( aShadowMap, currentDepth, st3, aShadowData.texIndex, shadow );
			}
			shadow /= 15.0;
		} else {
			shadow /= 9.0;
		}
	} else {
		int samples = 20;
		for(int i = 0; i < samples; ++i) {
			st3 = fragToLight + (gridSamplingDisk20_v3[i] * offset);
			AddShadowAmountCubeMap( aShadowMap, currentDepth, st3, aShadowData.texIndex, shadow );
		}
		shadow /= float(samples+1);
	}
	return shadow;
}
	
float PointLightShadow( in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal ) {
	// aWorldNormal should be normalized
	vec3 normalOffset = aWorldNormal*(aShadowData.normalBias);
	vec3 lightDiff = (aWorldFragPos+normalOffset) - aShadowData.lightWorldPos;
	float cosTheta = dot( aWorldNormal, normalize(lightDiff));
	float bias = max(aShadowData.bias * ( 1.0-abs(cosTheta )), aShadowData.bias * 0.05);
	float currentDepth = length(lightDiff) / aShadowData.far;
#ifdef SHADOWS_USE_CUBE_MAP_ARRAY
	float closestDepth = texture(uShadowCubeMap, vec4(lightDiff, aShadowData.texIndex) ).r;
#else
	float closestDepth = texture(uShadowCubeMap, lightDiff ).r;
#endif
	float diskRadius = (abs(currentDepth-closestDepth)/(aShadowData.far*0.5)) * aShadowData.sampleRadius + aShadowData.sampleRadius;
	float shadow = SampleShadowCube(aShadowData, uShadowCubeMap, lightDiff, currentDepth, closestDepth, diskRadius, bias);
	return shadow;
}

#ifdef SHADOWS_USE_TEXTURE_ARRAY
void AddShadowVisibility( in sampler2DArrayShadow aShadowMap, in vec3 acoords, in int aTexIndex, inout float visibility ) {
#else
void AddShadowVisibility( in sampler2DShadow aShadowMap, in vec3 acoords, in int aTexIndex, inout float visibility ) {
#endif
	acoords.xy = clamp( acoords.xy, shadowCoordsMin, shadowCoordsMax );
#ifdef SHADOWS_USE_TEXTURE_ARRAY
	visibility += texture(aShadowMap, vec4(acoords.xy, aTexIndex, acoords.z ));
#else
	visibility += texture(aShadowMap, vec3(acoords.xy, acoords.z ));
#endif
}
	
#ifdef SHADOWS_USE_TEXTURE_ARRAY
float SampleShadow(in shadowData aShadowData, in sampler2DArrayShadow aShadowMap, vec4 fragPosLightSpace, float offset, float abias ) {
#else
float SampleShadow(in shadowData aShadowData, in sampler2DShadow aShadowMap, vec4 fragPosLightSpace, float offset, float abias ) {
#endif
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	if (projCoords.x >= 1.0 || projCoords.x <= 0.0 ||
		projCoords.y >= 1.0 || projCoords.y <= 0.0 ||
		projCoords.z >= 1.0 || projCoords.z <= 0.0) {
		return 1.0;
	}
	float bias = abias;
	int shadowTexIndex = aShadowData.texIndex;
	float visibility = 1.0;
	projCoords.z = projCoords.z-bias;
#ifdef SHADOWS_USE_TEXTURE_ARRAY
	visibility = texture(aShadowMap, vec4(projCoords.xy, shadowTexIndex, projCoords.z ));
#else
	visibility = texture(aShadowMap, projCoords );
#endif
	vec3 st3 = projCoords.xyz;
	if( aShadowData.shadowType < 1.0 ) {
		//hard shadow, visibility already set above
	} else if( aShadowData.shadowType < 2.0 ) {
		float radius = offset;
		// sample from the corners
		for( int i = 0; i < 4; i++ ) {
			st3.xy = projCoords.xy+cornerSampleDisk4_v2[i]*radius;
			AddShadowVisibility( aShadowMap, st3, shadowTexIndex, visibility );
		}
		visibility /= 5.0;
	} else if( aShadowData.shadowType < 3.0 ) {
		float radius = offset;
		// sample from the corners
		for( int i = 0; i < 4; i++ ) {
			st3.xy = projCoords.xy+cornerSampleDisk4_v2[i]*radius;
			AddShadowVisibility( aShadowMap, st3, shadowTexIndex, visibility );
		}
		// sample from the up, down, left, right
		for( int i = 0; i < 4; i++ ) {
			st3.xy = projCoords.xy+cardinalSampleDisk4_v2[i]*radius;
			AddShadowVisibility( aShadowMap, st3, shadowTexIndex, visibility );
		}
		visibility /= 9.0;
	} else {
		float radius = offset;
		for( int i = 0; i < 16; i++ ) {
			st3.xy = projCoords.xy+poissonDisk16_v2[i]*radius;
			AddShadowVisibility( aShadowMap, st3, shadowTexIndex, visibility );
		}
		visibility /= 17.0;
	}
	return visibility;
}

#ifdef SHADOWS_USE_TEXTURE_ARRAY
void AddShadowAmountSampler2D( in sampler2DArray aShadowMap, in vec3 st3, in int aTexIndex, inout float ashadow ) {
#else
void AddShadowAmountSampler2D( in sampler2D aShadowMap, in vec3 st3, in int aTexIndex, inout float ashadow ) {
#endif
	st3.xy = clamp( st3.xy, shadowCoordsMin, shadowCoordsMax );
#ifdef SHADOWS_USE_TEXTURE_ARRAY
	float closestDepth = texture(aShadowMap, vec3(st3.xy, aTexIndex )).r;
#else
	float closestDepth = texture(aShadowMap, st3.xy ).r;
#endif
	if(closestDepth < 1.0 && st3.z > closestDepth) {
		ashadow += 1.f;
	}
}
		
#ifdef SHADOWS_USE_TEXTURE_ARRAY
float SampleShadow2D(in shadowData aShadowData, in sampler2DArray aShadowMap, vec3 aprojCoords, float aclosestDepth, float offset, float abias ) {
#else
float SampleShadow2D(in shadowData aShadowData, in sampler2D aShadowMap, vec3 aprojCoords, float aclosestDepth, float offset, float abias ) {
#endif
	float shadow = 0.0;
	float bias = abias;
	int shadowTexIndex = aShadowData.texIndex;
	vec3 projCoords = aprojCoords;
	projCoords.z = projCoords.z-bias;
	
	vec3 st3 = projCoords.xyz;
//	float currentDepth = projCoords.z;
	float closestDepth = aclosestDepth;
	
	if(closestDepth < 1.0 && st3.z > closestDepth) {
		shadow = 1.0;
	}

	if( aShadowData.shadowType < 1.0 ) {
		// hard shadow
	} else if( aShadowData.shadowType < 2.0 ) {
		float radius = offset;
		// sample from the corners
		for( int i = 0; i < 4; i++ ) {
			st3.xy = projCoords.xy+cornerSampleDisk4_v2[i]*radius;
			AddShadowAmountSampler2D( aShadowMap, st3, shadowTexIndex, shadow );
		}
		shadow /= 5.0;
	} else if( aShadowData.shadowType < 3.0 ) {
		float radius = offset;
		// sample from the corners
		for( int i = 0; i < 4; i++ ) {
			st3.xy = projCoords.xy+cornerSampleDisk4_v2[i]*radius;
			AddShadowAmountSampler2D( aShadowMap, st3, shadowTexIndex, shadow );
		}
		if( shadow < 5.0 ) {
			// sample from the up, down, left, right
			for( int i = 0; i < 4; i++ ) {
				st3.xy = projCoords.xy+cardinalSampleDisk4_v2[i]*radius;
				AddShadowAmountSampler2D( aShadowMap, st3, shadowTexIndex, shadow );
			}
			shadow /= 9.0;
		} else {
			shadow /= 5.0;
		}
	} else {
		float radius = offset;
		int filterSize = 12;
		radius = offset / (float(filterSize));
		for (int i = -filterSize; i <= filterSize; i++) {
			for (int j = -filterSize; j <= filterSize; j++) {
				st3.xy = projCoords.xy + vec2(float(i),float(j)) * radius;
				AddShadowAmountSampler2D( aShadowMap, st3, shadowTexIndex, shadow );
			}
		}
		shadow /= (1.0+((2.0 * float(filterSize) + 1.0) * (2.0 * float(filterSize) + 1.0)));
	}
	return shadow;
}
	
float getShadowBias(in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal) {
	vec3 lightDiff = aWorldFragPos - aShadowData.lightWorldPos;
	float cosTheta = dot(aWorldNormal, normalize(lightDiff));
	cosTheta = clamp(cosTheta, 0.0, 1.0);
	return clamp(aShadowData.bias * 0.1 * (1.0-(cosTheta)), aShadowData.bias * 0.01, aShadowData.bias);
}
			
float DirectionalShadow( in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal) {
	float bias = getShadowBias(aShadowData, aWorldFragPos, aWorldNormal);
	vec3 normalOffset = aWorldNormal*(aShadowData.normalBias);
	vec4 fragPosLightSpace = aShadowData.shadowMatrix * vec4(aWorldFragPos+normalOffset, 1.0);
	float visibility = SampleShadow( aShadowData, uShadowMapDirectional, fragPosLightSpace, aShadowData.sampleRadius, bias );
	return clamp(1.0-visibility, 0.0, 1.0);
}
			
float SpotShadow( in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal) {
	float bias = getShadowBias(aShadowData, aWorldFragPos, aWorldNormal);
	vec3 normalOffset = aWorldNormal*(aShadowData.normalBias);
	vec4 fragPosLightSpace = aShadowData.shadowMatrix * vec4(aWorldFragPos+normalOffset, 1.0);
	float visibility = SampleShadow( aShadowData, uShadowMapSpot, fragPosLightSpace, aShadowData.sampleRadius, bias );
	return clamp(1.0-visibility, 0.0, 1.0);
}
			
//https://www.gamedev.net/tutorials/programming/graphics/effect-area-light-shadows-part-1-pcss-r4971/
#ifdef SHADOWS_USE_TEXTURE_ARRAY
vec2 PCSS_BlockerDistance(in sampler2DArray atex, int aSampleIndex, in vec3 projCoord, in float searchUV, in vec2 rotationTrig) {
#else
vec2 PCSS_BlockerDistance(in sampler2D atex, int aSampleIndex, in vec3 projCoord, in float searchUV, in vec2 rotationTrig) {
#endif
	int PCSS_SampleCount = 6;
	// Perform N samples with pre-defined offset and random rotation, scale by input search size
	float blockers = 0.0;
	float avgBlocker = 0.0;
	int filterSize = PCSS_SampleCount;
	for (int i = -filterSize; i <= filterSize; i++) {
		for (int j = -filterSize; j <= filterSize; j++) {
			vec2 offset = vec2(float(i),float(j)) * searchUV;
#ifdef SHADOWS_USE_TEXTURE_ARRAY
			float z = texture(atex, vec3(projCoord.xy + offset, aSampleIndex) ).x;
#else
			float z = texture(atex, projCoord.xy + offset).x;
#endif
			if (z < projCoord.z) {
				blockers += 1.0;
				avgBlocker += z;
			}
		}
	}
	// Calculate average blocker depth
	if(blockers > 0.0) {
		avgBlocker /= blockers;
	}
	// To solve cases where there are no blockers - we output 2 values - average blocker depth and no. of blockers
	return vec2(avgBlocker, blockers);
}		
				
float AreaShadow( in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal, in vec2 lightSize) {
	float bias = getShadowBias(aShadowData, aWorldFragPos, aWorldNormal);
	vec3 normalOffset = aWorldNormal*(aShadowData.normalBias);
	vec4 fragPosLightSpace = aShadowData.shadowMatrix * vec4(aWorldFragPos, 1.0);
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	vec3 claMin = vec3(FLT_EPS);
	vec3 claMax = vec3(ONE_LESS_FLT_EPS);
	projCoords = clamp(projCoords, claMin, claMax);

#ifdef SHADOWS_USE_TEXTURE_ARRAY
	float closestDepth = texture(uShadowMapArea, vec3(projCoords.xy, aShadowData.texIndex) ).r;
#else
	float closestDepth = texture(uShadowMapArea, projCoords.xy ).r;
#endif
	float tsize = float(textureSize( uShadowMapArea, aShadowData.texIndex ).x);
	vec2 averageBlocker = PCSS_BlockerDistance(uShadowMapArea, aShadowData.texIndex, projCoords, (lightSize.x*0.5)/tsize, vec2(1.0,1.0));
	// If there isn't any average blocker distance - it means that there is no blocker at all
	if (averageBlocker.y < 1.0 ) {
		return 0.0f;
	}
	float averageBlockerDepth = averageBlocker.x;
	float receiverDepth = projCoords.z;
	float pvalue = (receiverDepth - averageBlockerDepth) / averageBlockerDepth;
	float penumbraSize = (clamp(abs(pvalue) * 4.0, 0.0, 1.0)) * aShadowData.sampleRadius + aShadowData.sampleRadius * 0.01;
	fragPosLightSpace = aShadowData.shadowMatrix * vec4(aWorldFragPos+normalOffset, 1.0);
	projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	float shadow = SampleShadow2D(aShadowData, uShadowMapArea, projCoords, closestDepth, penumbraSize, bias );
	return shadow;
}
)";






