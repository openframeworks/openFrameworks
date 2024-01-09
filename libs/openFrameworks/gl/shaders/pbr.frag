static const string shader_pbr_frag = R"(
IN vec3 v_worldPosition;
IN vec3 v_worldNormal;
IN vec2 v_texcoord; // receive the texCoord if needed

#if HAS_COLOR
IN vec4 v_color;
#endif

uniform vec3 uCameraPos;

#if !defined(PBR_QUALITY_LEVEL_LOW) && !defined(PBR_QUALITY_LEVEL_HIGH)
// set high level as default, no middle ground :0
#define PBR_QUALITY_LEVEL_HIGH
#endif

%custom_uniforms%

%shader_shadow_include%
%additional_includes%

%postFragment%

vec4 ofApplyPbr(in Material amat, in PbrData apbrData ) {
	// calculate the IBL
	#ifdef HAS_CUBE_MAP
	calcEnvironmentIndirect( apbrData, amat );
	#endif
	
	#if defined(MAX_LIGHTS) && MAX_LIGHTS
	// now lets add in all of the lights //
	for( int i = 0; i < MAX_LIGHTS; i++ ){
		if(lights[i].enabled<0.5) continue;
		// function calculates all the surface lighting for each light type, including shadows
		calcLight(lights[i], i, apbrData, amat);
	}
	#endif
	
	#ifndef HAS_CUBE_MAP
		#if defined(MAX_LIGHTS) && !MAX_LIGHTS
			// we have no lighting in the scene //
			// lets add some so we can see what's going on //
			#ifndef HAS_SHADOWS
			calcFakeDirectionalLight(uCameraPos, apbrData, amat );
			#endif
		#endif
	#endif
	
	vec3 totalDiffuse = apbrData.directDiffuse + apbrData.indirectDiffuse;
	vec3 totalSpecular = apbrData.directSpecular + apbrData.indirectSpecular;
	
	// now lets add in the global lighting //
	vec4 color  = vec4(0.0);
	color.rgb += totalDiffuse;
	color.rgb += totalSpecular;
	color.a = saturate(amat.albedo.a);
	
	color.rgb += saturate( amat.emissive.rgb * (amat.emissive.a * color.a));
	
	return linear2Gamma(color);
}

vec4 ofApplyPbr(in Material amat) {
	// make sure to setup material first //
	PbrData pbrData;
	setupPbrData( pbrData, amat, v_worldPosition, uCameraPos );
	return ofApplyPbr( amat, pbrData);
}

vec4 ofApplyPbr() {
	Material mat;
	mat.uv = v_texcoord;
	setupMaterial( mat );
	return ofApplyPbr(mat);
}

%mainFragment%

)";

// this is the default main function that can be overridden by the user
static const string shader_pbr_main_frag = R"(
void main() {
	vec4 color = ofApplyPbr();
	FRAG_COLOR = saturate(postFragment(color));
}
)";
