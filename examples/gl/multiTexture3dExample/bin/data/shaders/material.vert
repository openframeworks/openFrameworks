
// mapInfluence is added automatically by the material when calling
// mat->setCustomUniformTexture("mapInfluence", fboInfluence.getTexture() ); in ofApp::setup()
//uniform SAMPLER mapInfluence;
OUT float oInfluence;
OUT float oBump;

void main (void){
	vec2 tc = getTransformedTexcoord();
	
	oInfluence = 0.0;
	oBump = 0.0;
	vec4 npos = position;
#if defined(HAS_TEX_DISPLACEMENT)
	float influence = TEXTURE(mapInfluence, tc).r;
	float df = getVertexDisplacement();
	npos.xyz = vec3(normal.xyz * pow(influence, 3.0) * 0.5);
	npos.xyz += normal.xyz * df * influence * mat_displacement_strength;
	npos.xyz += position.xyz;
	oInfluence = influence;//clamp(influence,0.001,.1);
	oBump = df;
#endif
	
	sendVaryings(npos);
	gl_Position = modelViewProjectionMatrix * npos;
}
