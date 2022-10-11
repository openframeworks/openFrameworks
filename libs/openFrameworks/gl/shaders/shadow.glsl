
static const string shadowShaderInclude = R"(
struct shadowData
{
	float enabled;
	vec3 lightWorldPos;
	vec3 lightUp;
	vec3 lightRight;
	float near;
	float far;
	// 0 = hard, 1 = pcf 2x2, 2 = pcf 9 samples, 3 = pcf 20 samples
	float shadowType;
	
	int texIndex;
	
	vec4 color;
	float bias;
	float normalBias;
	float sampleRadius;
	
	mat4 shadowMatrix;
};
uniform shadowData shadows[MAX_LIGHTS];
#ifndef SHADOWS_EXCLUDE_CUBE_MAP_ARRAY
uniform samplerCubeArray uShadowCubeMap;
#endif
uniform sampler2DArrayShadow uShadowMapDirectional;
uniform sampler2DArrayShadow uShadowMapSpot;




vec2 cardinalSampleDisk4_v2[4] = vec2[]
(
 vec2(-1, 0), vec2(1, 0), vec2(0, -1), vec2(0, 1)
 );

vec2 cornerSampleDisk4_v2[4] = vec2[]
(
 vec2(-1, -1), vec2(1, -1), vec2(1, 1), vec2(-1, 1)
 );

//vec2 alignedSamplingDisk9_v2[9] = vec2[]
//(
// vec2(-1, -1), vec2(0, -1), vec2(1, -1),
// vec2(-1, 0), vec2(0, 0), vec2(1, 0),
// vec2(-1, 1), vec2(0, 1), vec2(1, 1)
// );

vec2 poissonDisk16_v2[16] = vec2[](
								   vec2( -0.94201624, -0.39906216 ),
								   vec2( 0.94558609, -0.76890725 ),
								   vec2( -0.094184101, -0.92938870 ),
								   vec2( 0.34495938, 0.29387760 ),
								   vec2( -0.91588581, 0.45771432 ),
								   vec2( -0.81544232, -0.87912464 ),
								   vec2( -0.38277543, 0.27676845 ),
								   vec2( 0.97484398, 0.75648379 ),
								   vec2( 0.44323325, -0.97511554 ),
								   vec2( 0.53742981, -0.47373420 ),
								   vec2( -0.26496911, -0.41893023 ),
								   vec2( 0.79197514, 0.19090188 ),
								   vec2( -0.24188840, 0.99706507 ),
								   vec2( -0.81409955, 0.91437590 ),
								   vec2( 0.19984126, 0.78641367 ),
								   vec2( 0.14383161, -0.14100790 )
								   );

// vec 3 arrays of ofset direction for sampling
vec3 cardinalSamplingDisk6_v3[6] = vec3[]
(
 vec3(0, 0, -1), vec3(0, 0, 1),
 vec3(-1, 0, 0), vec3(-1, 0, 0),
 vec3(0, -1, 0), vec3(0, 1, 0)
 );

vec3 cornerSamplingDisk8_v3[8] = vec3[]
(
 vec3(-1, -1, -1), vec3(1, -1, -1), vec3(1, 1, -1), vec3(-1, 1, -1),
 vec3(-1, -1, 1), vec3(1, -1, 1), vec3(1, 1, 1), vec3(-1, 1, 1)
 );

// array of offset direction for sampling
vec3 gridSamplingDisk20_v3[20] = vec3[]
(
 vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
 vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
 vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
 vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
 vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
 );



// https://github.com/patriciogonzalezvivo/lygia/blob/main/generative/random.glsl#L15
float random(in vec2 st) {
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}


#ifndef SHADOWS_EXCLUDE_CUBE_MAP_ARRAY
float SampleShadowCube(in shadowData aShadowData, in samplerCubeArray aShadowMap, vec3 fragToLight, float acurrentDepth, float aclosestDepth, float offset, float abias ) {
	float shadow = 0.0;
	
	float currentDepth = acurrentDepth;
	float closestDepth = aclosestDepth;
	
	
	if(closestDepth < 1.0 && currentDepth - abias > closestDepth) {
		shadow = 1.f;
	}
	
	if( aShadowData.shadowType < 1 ) {
		// already set shadow above
	} else if( aShadowData.shadowType < 2 ) {
		for( int i = 0; i < 8; i++ ) {
			closestDepth = texture(uShadowCubeMap, vec4(fragToLight + (cornerSamplingDisk8_v3[i] * offset), aShadowData.texIndex) ).r;
			if(closestDepth < 1.0 && currentDepth - abias > closestDepth) {
				shadow += 1.0;
			}
		}
		// dividing by 9 because shadow was set initially and added a grid of 8
		shadow /= 9.0;
	} else if( aShadowData.shadowType < 3 ) {
		int samples = 8;
		for(int i = 0; i < samples; ++i) {
			closestDepth = texture(aShadowMap, vec4(fragToLight + (cornerSamplingDisk8_v3[i] * offset), aShadowData.texIndex) ).r;
			if(closestDepth < 1.0 && currentDepth - abias > closestDepth) {
				shadow += 1.0;
			}
		}
		if( shadow > 0 ) {
			int samples = 6;
			for(int i = 0; i < samples; ++i) {
				closestDepth = texture(aShadowMap, vec4(fragToLight + (cardinalSamplingDisk6_v3[i] * offset), aShadowData.texIndex) ).r;
				if(closestDepth < 1.0 && currentDepth - abias > closestDepth) {
					shadow += 1.0;
				}
			}
			shadow /= 15.0;
		} else {
			shadow /= 9.0;
		}
	} else {
		int samples = 20;
		for(int i = 0; i < samples; ++i) {
			closestDepth = texture(aShadowMap, vec4(fragToLight + (gridSamplingDisk20_v3[i] * offset), aShadowData.texIndex) ).r;
			if(closestDepth < 1.0 && currentDepth - abias > closestDepth) {
				shadow += 1.0;
			}
		}
		shadow /= float(samples+1.0);
	}
	return shadow;
}
#endif

float PointLightShadow( in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal ) {
	// aWorldNormal should be normalized
#ifndef SHADOWS_EXCLUDE_CUBE_MAP_ARRAY
	vec3 normalOffset = aWorldNormal*(aShadowData.normalBias);
	vec3 lightDiff = (aWorldFragPos+normalOffset) - aShadowData.lightWorldPos;
	
	float cosTheta = dot( aWorldNormal, normalize(lightDiff));
	float bias = max(aShadowData.bias * ( 1.0-abs(cosTheta )), aShadowData.bias * 0.05);
	
	float currentDepth = length(lightDiff) / aShadowData.far;
	float closestDepth = texture(uShadowCubeMap, vec4(lightDiff, aShadowData.texIndex) ).r;
	
	float diskRadius = (abs(currentDepth-closestDepth)/(aShadowData.far*0.5)) * aShadowData.sampleRadius + aShadowData.sampleRadius * 0.5;

	float shadow = SampleShadowCube(aShadowData, uShadowCubeMap, lightDiff, currentDepth, closestDepth, diskRadius, bias);
	return shadow;
#endif
	return 0.0;
}


float SampleShadow(in shadowData aShadowData, in sampler2DArrayShadow aShadowMap, vec4 fragPosLightSpace, float offset, float abias ) {
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
	
	if( aShadowData.shadowType < 1 ) {
		// hard shadow //
		visibility = texture(aShadowMap, vec4(projCoords.xy, shadowTexIndex, projCoords.z-bias ));
	} else if( aShadowData.shadowType < 2 ) {
		visibility = texture(aShadowMap, vec4(projCoords.xy, shadowTexIndex, projCoords.z-bias ));
		float radius = offset;
		// sample from the corners
		for( int i = 0; i < 4; i++ ) {
			visibility += texture(aShadowMap, vec4(projCoords.xy+cornerSampleDisk4_v2[i]*radius, shadowTexIndex, projCoords.z-bias ));
		}
		visibility /= 5.0;
	} else if( aShadowData.shadowType < 3 ) {
		visibility = texture(aShadowMap, vec4(projCoords.xy, shadowTexIndex, projCoords.z-bias ));
		float radius = offset;
		// sample from the corners
		for( int i = 0; i < 4; i++ ) {
			visibility += texture(aShadowMap, vec4(projCoords.xy+cornerSampleDisk4_v2[i]*radius, shadowTexIndex, projCoords.z-bias ));
		}
		if( visibility < 5.0 ) {
			// sample from the up, down, left, right
			for( int i = 0; i < 4; i++ ) {
				visibility += texture(aShadowMap, vec4(projCoords.xy+cardinalSampleDisk4_v2[i]*radius, shadowTexIndex, projCoords.z-bias ));
			}
			visibility /= 9.0;
		} else {
			visibility /= 5.0;
		}
	} else {
		float radius = offset;
		visibility = texture(aShadowMap, vec4(projCoords.xy, shadowTexIndex, projCoords.z-bias ));
		for( int i = 0; i < 16; i++ ) {
			visibility += texture(aShadowMap, vec4(projCoords.xy+poissonDisk16_v2[i]*radius, shadowTexIndex, projCoords.z-bias ));
		}
		visibility /= 17;
	}
	
	return visibility;
}

float DirectionalShadow( in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal) {
	// aWorldNormal should be normalized //
	vec3 lightDiff = aWorldFragPos - aShadowData.lightWorldPos;
	float cosTheta = dot(aWorldNormal, normalize(lightDiff));
	cosTheta = clamp(cosTheta, 0.0, 1.0);
	float bias = aShadowData.bias;
	//	float bias = 0.05;
	bias = clamp(aShadowData.bias*tan(acos(cosTheta)), aShadowData.bias, aShadowData.bias*5.0); // cosTheta is dot( n,l ), clamped between 0 and 1
	vec3 normalOffset = aWorldNormal*(aShadowData.normalBias);
	
	vec4 fragPosLightSpace = aShadowData.shadowMatrix * vec4(aWorldFragPos+normalOffset, 1.0);
	float visibility = SampleShadow( aShadowData, uShadowMapDirectional, fragPosLightSpace, aShadowData.sampleRadius, bias );
	return clamp(1.0-visibility, 0.0, 1.0);
}
	
float SpotShadow( in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal) {
	
	vec3 lightDiff = aWorldFragPos - aShadowData.lightWorldPos;
	float cosTheta = dot(aWorldNormal, normalize(lightDiff));
	float bias = clamp(aShadowData.bias * 0.1 * (1.0-(cosTheta)), aShadowData.bias * 0.01, aShadowData.bias);
	
	vec3 normalOffset = aWorldNormal*(aShadowData.normalBias);
	vec4 fragPosLightSpace = aShadowData.shadowMatrix * vec4(aWorldFragPos+normalOffset, 1.0);
	
	float visibility = SampleShadow( aShadowData, uShadowMapSpot, fragPosLightSpace, aShadowData.sampleRadius, bias );
	return clamp(1.0-visibility, 0.0, 1.0);
}
)";






