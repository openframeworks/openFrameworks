
IN float oInfluence;
IN float oBump;

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
	
	FRAG_COLOR = saturate(postFragment(color));
}


