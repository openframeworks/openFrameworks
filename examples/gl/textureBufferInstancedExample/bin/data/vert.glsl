#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec4 instanceColor;
uniform samplerBuffer tex;

#if !defined(OF_SHADOW_DEPTH_PASS)
out vec4 color;
#endif

void main(){
    int x = gl_InstanceID*4;
    
    mat4 transformMatrix = mat4( 
        texelFetch(tex, x),
        texelFetch(tex, x+1),
        texelFetch(tex, x+2), 
        texelFetch(tex, x+3)
    );
	
	#if defined(OF_SHADOW_DEPTH_PASS)
		vec3 worldPosition = (transformMatrix * vec4(position.xyz, 1.0)).xyz;
		sendShadowDepthWorldPosition(worldPosition);
	#else
		color = instanceColor;
		gl_Position = modelViewProjectionMatrix * transformMatrix * position;
	#endif
}
