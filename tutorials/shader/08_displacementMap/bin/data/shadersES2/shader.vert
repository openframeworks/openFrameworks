
// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
attribute vec4 position;
attribute vec2 texcoord;

varying vec2 texCoordVarying;

// PLEASE NOTE.
// texture2D() is not supported in vertex ES2 shaders.
// so unfortunately on ES2, we can not sample the texture,
// and displace the mesh.
// Please see GL2 and GL3 examples for displacement mapping.

void main()
{
    texCoordVarying = texcoord;
	gl_Position = modelViewProjectionMatrix * position;
}