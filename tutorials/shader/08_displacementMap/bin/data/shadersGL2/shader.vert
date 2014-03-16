#version 120

uniform sampler2DRect tex0;

varying vec3 texCoordVarying;

void main()
{
    // get the texture coordinates
    texCoordVarying = gl_MultiTexCoord0.xyz;

    // get the position of the vertex relative to the modelViewProjectionMatrix
    vec4 position = gl_Vertex;
    vec4 modifiedPosition = position;
    
    // we need to scale up the values we get from the texture
    //float scale = 0.00099125;
    
    // here we get the red channel value from the texture
    // to use it as vertical displacement
//    float displacementX = texture2DRect(tex0, texCoordVarying).x;
//    float displacementY = texture2DRect(tex0, texCoordVarying).y;
//    float displacementZ = (displacementX+displacementY)/2.0;
		
    // use the displacement we created from the texture data
    // to modify the vertex position
    //modifiedPosition.y += displacementY * scale;
    //modifiedPosition.z += displacementZ * scale;
    //modifiedPosition.z += displacementZ * scale;
    // this is the resulting vertex position
	gl_Position = modifiedPosition;
}