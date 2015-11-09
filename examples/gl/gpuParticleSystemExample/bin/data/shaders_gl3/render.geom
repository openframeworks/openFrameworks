#version 150

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 6) out;

uniform mat4 modelViewProjectionMatrix;

uniform float size;
uniform float imgWidth, imgHeight;

in vec4 vPosition[];

out vec2 vTexCoord;

void main(void){

    // For each vertex moved to the right position on the vertex shader
    // it makes 6 more vertex that makes 2 GL_TRIANGLE_STRIP
    // that´s going to be the frame for the pixels of the sparkImg texture
    //
    for(int i = 0; i < gl_in.length(); i++){
        gl_Position = modelViewProjectionMatrix * ( vPosition[i] + vec4(-size,-size,0.0,0.0));
        vTexCoord.x = 0.0;
        vTexCoord.y = 0.0;
        EmitVertex();
        
        gl_Position = modelViewProjectionMatrix * (vPosition[i] + vec4(size,-size,0.0,0.0));
        vTexCoord.x = imgWidth;
        vTexCoord.y = 0.0;
        EmitVertex();
        
        gl_Position = modelViewProjectionMatrix * (vPosition[i] + vec4(size,size,0.0,0.0));
        vTexCoord.x = imgWidth;
        vTexCoord.y = imgHeight;
        EmitVertex();
        EndPrimitive();

        gl_Position = modelViewProjectionMatrix * (vPosition[i] + vec4(-size,-size,0.0,0.0));
        vTexCoord.x = 0.0;
        vTexCoord.y = 0.0;
        EmitVertex();
        
        gl_Position = modelViewProjectionMatrix * (vPosition[i] + vec4(-size,size,0.0,0.0));
        vTexCoord.x = 0.0;
        vTexCoord.y = imgHeight;
        EmitVertex();
        
        gl_Position = modelViewProjectionMatrix * (vPosition[i] + vec4(size,size,0.0,0.0));
        vTexCoord.x = imgWidth;
        vTexCoord.y = imgHeight;
        EmitVertex();
        EndPrimitive();
    }
}