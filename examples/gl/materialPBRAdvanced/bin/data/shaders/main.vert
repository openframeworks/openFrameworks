
// the mesh we are using does not have texture coordinates
// so we are going to send the local vertex positions to use for animation
OUT vec3 v_localPos;

void main (void){
	v_localPos = position.xyz;
	vec4 npos = getTransformedPosition();
//	npos.xyz += vec3( cos(iElapsedTime+v_localPos.y*1.85), 0.0, 0.0);
	sendVaryings(npos);
	gl_Position = modelViewProjectionMatrix * npos;
}
