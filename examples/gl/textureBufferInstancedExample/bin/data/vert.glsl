#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec4 instanceColor;
uniform samplerBuffer tex;
out vec4 color;


void main(){
    int x = gl_InstanceID*4;
    
    mat4 transformMatrix = mat4( 
        texelFetch(tex, x),
        texelFetch(tex, x+1),
        texelFetch(tex, x+2), 
        texelFetch(tex, x+3)
    );
    color = instanceColor;
	gl_Position = modelViewProjectionMatrix * transformMatrix * position;
}