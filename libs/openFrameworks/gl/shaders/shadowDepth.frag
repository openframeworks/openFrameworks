
static const std::string depthFragShaderSource = R"(

uniform vec3 uLightPos;
uniform float uNearPlane;
uniform float uFarPlane;

#ifdef CUBE_MAP_SINGLE_PASS
in vec4 FragPos;
#endif

#if defined(CUBE_MAP_MULTI_PASS)
in vec3 v_worldPosition;
#endif

void main() {
#if defined(SINGLE_PASS)
	gl_FragDepth = gl_FragCoord.z;
#endif
	
#ifdef CUBE_MAP_SINGLE_PASS
	float lightDistance = length(FragPos.xyz - uLightPos);
	lightDistance = lightDistance / uFarPlane;
	// write out modified depth
	gl_FragDepth = lightDistance;
#endif
	
#if defined(CUBE_MAP_MULTI_PASS)
	float lightDistance = length(v_worldPosition.xyz - uLightPos);
	lightDistance = lightDistance / uFarPlane;
	gl_FragDepth = lightDistance;
#endif
	
}
)";
