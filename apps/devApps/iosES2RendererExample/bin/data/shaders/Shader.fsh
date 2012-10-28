#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec4 colorVarying;

void main()
{
	gl_FragColor = colorVarying;
}
