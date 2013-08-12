#version 120

varying vec4 globalColor;
uniform sampler2DRect tex0;

void main()
{
    // get the texture coordinates
    gl_TexCoord[0] = gl_MultiTexCoord0;

    // get the position of the vertex relative to the modelViewProjectionMatrix
    vec4 position = ftransform();
    vec4 modifiedPosition = position;
    
    // we need to scale up the values we get from the texture
    float scale = 100;
    
    // here we get the red channel value from the texture
    // to use it as vertical displacement
    float displacementY = texture2DRect(tex0, gl_TexCoord[0].st).x;
		
    // use the displacement we created from the texture data
    // to modify the vertex position
    modifiedPosition.y += displacementY * scale;

    // this is the resulting vertex position
	gl_Position = modifiedPosition;
}