#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect posTex;
uniform vec2 screen;
uniform float resolution;

void main() {
    // Takes the position of a vertex that was send on porpuse to the same  
    // position on the texture were it´s the information stored on the Red Green channels
    //
    vec4 verPos = gl_Vertex;
    
    // Moves the position of each vertex (that are from -1.0 to 1.0) 
    // to the right one on the texture (that are from 0.0 to 1.0)
    //
    verPos.x = abs(verPos.x * 0.5 + resolution * 0.5);
    verPos.y = abs(verPos.y * 0.5 + resolution * 0.5);
    vec4 pixPos = texture2DRect( posTex, verPos.xy );
    
    // Maps the position from the texture (from 0.0 to 1.0) to
    // the screen position (0 - screenWidth/screenHeight)
    //
    pixPos.x *= screen.x;
    pixPos.y *= screen.y;
    
    gl_Position = pixPos;
    gl_FrontColor =  gl_Color;
}