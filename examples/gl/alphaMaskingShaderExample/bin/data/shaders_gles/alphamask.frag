#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

uniform sampler2D tex0;
uniform sampler2D maskTex;
uniform vec4 globalColor;

varying vec2 texCoordVarying;

void main (void)
{
	vec2 pos = texCoordVarying;
	vec3 src = texture2D(tex0, pos).rgb;
	float mask = texture2D(maskTex, pos).r;
	gl_FragColor = vec4( src , mask);
}
