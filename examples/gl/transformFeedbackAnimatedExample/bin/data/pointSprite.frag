OF_GLSL_SHADER_HEADER

out vec4 FRAGCOLOR;

uniform sampler2D tex;

//-- coming from the vert shader ---- //
in vec4 oColor;
// ---------------------------------- //

void main(){
	// gl_PointCoord is provided by openGL when point sprites are enabled
	// used for texture coords 
	vec4 texColor = texture( tex, gl_PointCoord.xy );
	FRAGCOLOR = oColor * texColor;
}
