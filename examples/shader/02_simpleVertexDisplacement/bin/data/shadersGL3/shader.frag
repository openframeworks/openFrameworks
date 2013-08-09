#version 150

out vec4 outputColor;
 
void main()
{
	float r = gl_FragCoord.x / 1024.; // divided by our window width
	float g = gl_FragCoord.y/ 768.; // divided by our window height
	float b = 0.; // zero so we get a nice fade from green -> red
	float a = 1.0; // full alpha (0 = transparent, 1 = full alpha)
    outputColor = vec4(r, g, b, a);
}