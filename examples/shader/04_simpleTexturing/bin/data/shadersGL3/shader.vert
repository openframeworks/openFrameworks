#version 150

// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;
// this is the end of the default functionality

// this is something we're creating
out vec2 varyingtexcoord;

// this is going to be passed in our program
uniform float time;


void main()
{
	vec4 modifiedPosition = modelViewProjectionMatrix * position;
	modifiedPosition.y += (sin(time + (modifiedPosition.x/100)) * 100 );

    varyingtexcoord = (textureMatrix * vec4(texcoord.x,texcoord.y,0,1)).xy *
        (1 + sin(time)/8);
    
	gl_Position = modifiedPosition;
}