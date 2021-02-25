
// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
attribute vec4 position;
attribute vec2 texcoord;

uniform sampler2D tex0;

varying vec2 texCoordVarying;

void main()
{
	// get the texture coordinates
    texCoordVarying = texcoord;

    // get the position of the vertex relative to the modelViewProjectionMatrix
	vec4 modifiedPosition = modelViewProjectionMatrix * position;
   
    // we need to scale up the values we get from the texture
    float scale = 100.0;
    
    // here we get the red channel value from the texture
    // to use it as vertical displacement
    float displacementY = texture2DLod(tex0, texCoordVarying.xy, 0.0).x;
		
    // use the displacement we created from the texture data
    // to modify the vertex position
    modifiedPosition.y += displacementY * scale;

    // this is the resulting vertex position
	gl_Position = modifiedPosition;
}
