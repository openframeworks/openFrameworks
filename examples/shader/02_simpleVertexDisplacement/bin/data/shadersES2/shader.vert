
attribute vec4 position;

uniform mat4 modelViewProjectionMatrix;

// the time value is passed into the shader by the OF app.
uniform float time;

void main()
{
    // the sine wave travels along the x-axis (across the screen),
    // so we use the x coordinate of each vertex for the calculation,
    // but we displace all the vertex along the y axis (up the screen)/
    float displacementHeight = 100.0;
    float displacementY = sin(time + (position.x / 100.0)) * displacementHeight;
	
    vec4 modifiedPosition = modelViewProjectionMatrix * position;
	modifiedPosition.y += displacementY;
	gl_Position = modifiedPosition;
}