// we set this define in the ofApp class
#if defined(NON_MATERIAL_DEPTH_PASS)
uniform mat4 modelMatrix;
in vec4 position;
uniform float iElapsedTime;
uniform float uWiggleVerts;
#endif

// this define is added via ofShadow.setupShadowDepthShader
#if !defined(OF_SHADOW_DEPTH_PASS)
// the mesh we are using does not have texture coordinates
// so we are going to send the local vertex positions to use for animation
OUT vec3 v_localPos;
#endif


void main (void){
	vec4 npos = position;
	npos.xyz += uWiggleVerts * 0.25 * vec3( cos(iElapsedTime*1.3+(position.z*1.25)+position.y*1.25), 0.0, 0.0);
#if defined(OF_SHADOW_DEPTH_PASS)
	vec3 worldPosition = (modelMatrix * vec4(npos.xyz, 1.0)).xyz;
	// this method is added via ofShadow.setupShadowDepthShader
	sendShadowDepthWorldPosition(worldPosition);
#else
	v_localPos = position.xyz;
	// this method is added via ofMaterial
	sendVaryings(npos);
	gl_Position = modelViewProjectionMatrix * npos;
#endif
}
