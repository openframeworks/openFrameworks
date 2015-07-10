#version 150

uniform sampler2DRect backbuffer;   // previous velocity texture
uniform sampler2DRect posData;      // position texture

uniform float timestep;

in vec2 vTexCoord;

out vec4 vFragColor;
    
void main(void){
    // Get the position and velocity from the pixel color.
    vec2 pos = texture( posData, vTexCoord).xy;
    vec2 vel = texture( backbuffer, vTexCoord ).xy;
        
    // Calculate what´s going to be the next position without updating it,
    // to see if it collide with the borders of the FBO texture.
    vec2 nextPos = pos; 
    nextPos += vel * timestep;
        
    // If it´s going to collide, change the velocity course.
    if ( nextPos.x < 0.0)
        vel.x = 0.5 * abs(vel.x);
        
    if ( nextPos.x > 1.0)
        vel.x = -0.5 * abs(vel.x);
        
    if (nextPos.y < 0.0)
        vel.y = 0.5 * abs(vel.y);
    
    if ( nextPos.y > 1.0)
        vel.y = -0.5 * abs(vel.y);
    
    // Then save the vel data into the velocity FBO.
    vFragColor = vec4(vel.x,vel.y,0.0,1.0);
}