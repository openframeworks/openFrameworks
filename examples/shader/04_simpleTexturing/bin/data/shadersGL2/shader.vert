#version 120

// this is coming from our C++ code
uniform float mouseX;

void main()
{

    // these are passed in by default from OpenGL
    vec4 vertex = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec2 texcoord = gl_MultiTexCoord0.xy;

    // here we move the texture coordinates
    gl_TexCoord[0] = vec4(texcoord.x + mouseX, texcoord.y, 0, 0);

    // send the vertices to the fragment shader
	gl_Position = vertex;
}