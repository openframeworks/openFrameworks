
IN float oInfluence;
IN float oBump;

//uniform float uIsDrawingInto;

void main() {
	
	Material mat;
	mat.uv = v_texcoord;
	setupMaterial( mat );
	// grab the emissive color from the material
	vec3 innerColor = mat.emissive.rgb;
	// remove the emissive color on the material so it's not applied in PBR function
	mat.emissive = vec4(0.0,0.0,0.0,1.0);
	vec4 color = ofApplyPbr(mat);
	
	color.rgb = mix( innerColor, color.rgb, clamp(pow(oInfluence, 0.5),0.0,1.0) * pow(clamp(oBump*2.0, 0.0, 1.0), 0.3) );
	
	float mixAmnt = 0.0;
#if defined(HAS_TEX_DISPLACEMENT)
	float brushInf = TEXTURE(mapInfluence, mat.uv).g;
	mixAmnt = brushInf * 0.7;
#endif
	color.rgb = mix(color.rgb, vec3(1,1,1), mixAmnt * uIsDrawingInto );
	
	FRAG_COLOR = saturate(postFragment(color));
}


