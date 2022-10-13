
static const std::string depthFragShaderSource = R"(

//#version 330
uniform vec3 uLightPos;

#ifdef CUBE_MAP_SINGLE_PASS
uniform float uNearPlane;
uniform float uFarPlane;

in vec4 FragPos;
#endif

#ifdef CUBE_MAP_MULTI_PASS
uniform float uNearPlane;
uniform float uFarPlane;

in vec3 v_worldPosition;
#endif

void main() {
#ifdef SINGLE_PASS
	gl_FragDepth = gl_FragCoord.z;
#endif
	
#ifdef CUBE_MAP_SINGLE_PASS
	float lightDistance = length(FragPos.xyz - uLightPos);
	lightDistance = lightDistance / uFarPlane;
	// write out modified depth
	gl_FragDepth = lightDistance;
#endif
	
#ifdef CUBE_MAP_MULTI_PASS
	float lightDistance = length(v_worldPosition.xyz - uLightPos);
	lightDistance = lightDistance / uFarPlane;
	gl_FragDepth = lightDistance;
#endif
	
}
)";
