#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
uniform samplerBuffer tex;
uniform int numInstances;

void main(){
    int x = gl_InstanceID*4;
    
    mat4 transformMatrix = mat4( 
        texelFetch(tex, x),
        texelFetch(tex, x+1),
        texelFetch(tex, x+2), 
        texelFetch(tex, x+3)
    );
	gl_Position = modelViewProjectionMatrix * transformMatrix * position;
}