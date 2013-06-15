#version 150

uniform sampler2DRect prevPosData;  // recive the previus position texture
uniform sampler2DRect velData;      // recive the velocity texture

in vec2 texCoordVarying;
out vec4 fragColor;

uniform float timestep;

void main(void){
    vec2 st = texCoordVarying;    // gets the position of the pixel that it´s dealing with...
    
    vec2 pos = texture( prevPosData, st ).xy; // ... in order to look at a particulary place on it
    vec2 vel = texture( velData, st ).xy;     // Fetch both the pos and vel.
    
    pos += vel * timestep; // Updates the position
    
    fragColor = vec4(pos.x,pos.y,1.0,1.0);  // And finaly it store it on the position FBO
}