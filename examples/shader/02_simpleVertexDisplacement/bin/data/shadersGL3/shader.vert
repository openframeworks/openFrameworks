#version 150

// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
in vec4 position;

// this is going to be passed in our program
uniform float time;


void main()
{
	vec4 modifiedPosition = modelViewProjectionMatrix * position;
	modifiedPosition.y += (sin(modifiedPosition.x + time) * 100);
	gl_Position = modifiedPosition;
}