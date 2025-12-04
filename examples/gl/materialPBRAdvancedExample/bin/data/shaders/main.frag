//uniform float iElapsedTime;
//Silver	0.97, 0.96, 0.91	#f7f4e8
//
//Aluminum	0.91, 0.92, 0.92	#e8eaea
//
//Titanium	0.76, 0.73, 0.69	#c1baaf
//
//Iron	0.77, 0.78, 0.78	#c4c6c6
//
//Platinum	0.83, 0.81, 0.78	#d3cec6
//
//Gold	1.00, 0.85, 0.57	#ffd891
//
//Brass	0.98, 0.90, 0.59	#f9e596
//
//Copper	0.97, 0.74, 0.62	#f7bc9e

// sent over from the main.vert shader
IN vec3 v_localPos;

void main() {
	Material mat;
	mat.uv = v_texcoord;
	setupMaterial( mat );

	vec4 gold = vec4(1.00, 0.85, 0.57, 1.0);
	vec4 silver = vec4(0.97, 0.96, 0.91, 1.0);
	vec4 pink = vec4(0.85, 0.16, 0.43, 1.0);
	vec4 green = vec4(0.1,0.86,0.54,1.0);
	
	float sstep = 0.5;

	sstep = smoothstep(0.4, 0.6, sin(v_localPos.z*0.9-iElapsedTime*5.3f));
	// try uncommenting the following lines for different animation
//	sstep = smoothstep(0.1, 0.9, sin(v_localPos.y*abs(cos(iElapsedTime*0.001))-iElapsedTime*4.f));
//	sstep = smoothstep(0.4, 0.6, sin((v_localPos.z+v_localPos.x)-iElapsedTime*5.3f));
//	sstep = max( float(mod(v_localPos.z+v_localPos.x-iElapsedTime, 1.0) < 0.15), float(mod(v_localPos.z-v_localPos.x+iElapsedTime, 1.0) < 0.15));
	
	mat.metallic = sstep;
	mat.roughness = (1.0-sstep)*0.75+0.19;
	mat.albedo = mix(mat.albedo, mix(gold, silver, (cos(iElapsedTime)*0.5+0.5)*2.0), sstep );
	// try uncommenting the following line for different colors
//	mat.albedo = mix(pink, green, sstep );
//	mat.albedo = mix(mat.albedo, green, sstep );
//	mat.albedo = mix(mat.albedo, vec4(silver.rgb*0.1,1.0), sstep );
	
	vec4 color = ofApplyPbr(mat);
	FRAG_COLOR = saturate(postFragment(color));
}
