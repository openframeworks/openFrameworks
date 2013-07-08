#version 150
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

uniform float size;
uniform float imgWidth, imgHeight;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

out vec2 texCoordVarying;

void main(void){

    // For each vertex moved to the right position on the vertex shader
    // it makes 6 more vertex that makes 2 GL_TRIANGLE_STRIP
    // that´s going to be the frame for the pixels of the sparkImg texture
    //
    for(int i = 0; i < gl_VerticesIn; i++){
        gl_Position = modelViewProjectionMatrix * ( gl_PositionIn[i] + vec4(-size,-size,0.0,0.0));
        texCoordVarying.x = 0.0;
        texCoordVarying.y = 0.0;
        EmitVertex();
        
        gl_Position = modelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,-size,0.0,0.0));
        texCoordVarying.x = imgWidth;
        texCoordVarying.y = 0.0;
        EmitVertex();
        
        gl_Position = modelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,size,0.0,0.0));
        texCoordVarying.x = imgWidth;
        texCoordVarying.y = imgHeight;
        EmitVertex();
        EndPrimitive();

        gl_Position = modelViewProjectionMatrix * (gl_PositionIn[i] + vec4(-size,-size,0.0,0.0));
        texCoordVarying.x = 0.0;
        texCoordVarying.y = 0.0;
        EmitVertex();
        
        gl_Position = modelViewProjectionMatrix * (gl_PositionIn[i] + vec4(-size,size,0.0,0.0));
        texCoordVarying.x = 0.0;
        texCoordVarying.y = imgHeight;
        EmitVertex();
        
        gl_Position = modelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,size,0.0,0.0));
        texCoordVarying.x = imgWidth;
        texCoordVarying.y = imgHeight;
        EmitVertex();
        EndPrimitive();
    }
}