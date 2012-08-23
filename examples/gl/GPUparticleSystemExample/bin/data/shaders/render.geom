#version 120
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

uniform float size;
uniform float imgWidth, imgHeight;

void main(void){

    // For each vertex moved to the right position on the vertex shader
    // it makes 6 more vertex that makes 2 GL_TRIANGLE_STRIP
    // that´s going to be the frame for the pixels of the sparkImg texture
    //
    for(int i = 0; i < gl_VerticesIn; i++){
        gl_Position = gl_ModelViewProjectionMatrix * ( gl_PositionIn[i] + vec4(-size,-size,0.0,0.0));
        gl_TexCoord[0].x = 0.0;
        gl_TexCoord[0].y = 0.0;
        EmitVertex();
        
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,-size,0.0,0.0));
        gl_TexCoord[0].x = imgWidth;
        gl_TexCoord[0].y = 0.0;
        EmitVertex();
        
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,size,0.0,0.0));
        gl_TexCoord[0].x = imgWidth;
        gl_TexCoord[0].y = imgHeight;
        EmitVertex();
        EndPrimitive();

        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(-size,-size,0.0,0.0));
        gl_TexCoord[0].x = 0.0;
        gl_TexCoord[0].y = 0.0;
        EmitVertex();
        
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(-size,size,0.0,0.0));
        gl_TexCoord[0].x = 0.0;
        gl_TexCoord[0].y = imgHeight;
        EmitVertex();
        
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,size,0.0,0.0));
        gl_TexCoord[0].x = imgWidth;
        gl_TexCoord[0].y = imgHeight;
        EmitVertex();
        EndPrimitive();
    }
}