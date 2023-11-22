
static const std::string depthVertexShaderSource = R"(

// depth camera's view projection matrix
#if defined(SINGLE_PASS) || defined(CUBE_MAP_MULTI_PASS)
uniform mat4 lightsViewProjectionMatrix;
#endif

#if defined(CUBE_MAP_MULTI_PASS)
out vec3 v_worldPosition;
#endif

void sendShadowDepthWorldPosition(in vec3 aWorldPos) {
#if defined(SINGLE_PASS)
	gl_Position = lightsViewProjectionMatrix * vec4(aWorldPos, 1.0);
#endif
	
#ifdef CUBE_MAP_SINGLE_PASS
	gl_Position = vec4(aWorldPos, 1.0);
#endif
	
#if defined(CUBE_MAP_MULTI_PASS)
	v_worldPosition = aWorldPos;
	gl_Position = lightsViewProjectionMatrix * vec4(aWorldPos, 1.0);
#endif
}

void sendShadowDepthWorldPosition(in vec4 aWorldPos) {
	sendShadowDepthWorldPosition(aWorldPos.xyz);
}

)";


static const std::string depthVertexShader_Main = R"(
in vec4 position;

// these are passed in from OF programmable renderer
uniform mat4 modelMatrix;

void main() {
	vec3 worldPosition = (modelMatrix * vec4(position.xyz, 1.0)).xyz;
	sendShadowDepthWorldPosition(worldPosition);
}

)";
