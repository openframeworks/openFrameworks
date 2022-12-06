static const string shader_pbr_vert = R"(
OUT vec3 v_worldPosition;
OUT vec3 v_worldNormal;
OUT vec2 v_texcoord; // pass the texCoord just in case
#if HAS_COLOR
OUT vec4 v_color;
#endif

%additional_includes%

IN vec4 position;
IN vec4 color;
IN vec4 normal;
IN vec2 texcoord;

// these are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform vec2 mat_texcoord_scale;

#if defined(HAS_TEX_DISPLACEMENT)
uniform float mat_displacement_strength;
#endif

vec2 getTransformedTexcoord() {
	return (textureMatrix*vec4(texcoord.x*mat_texcoord_scale.x,texcoord.y*mat_texcoord_scale.y,0,1)).xy;
}

vec4 getTransformedPositionAlongNormal(in float ainfluence) {
	vec3 npos = position.xyz;
	npos = vec3(normal.xyz * ainfluence);
	npos += position.xyz;
	return vec4(npos.xyz, position.w);
}

float getVertexDisplacement() {
	float df = 0.0;
#if defined(HAS_TEX_DISPLACEMENT)
	vec4 dispColor = TEXTURE(tex_displacement, texcoord * mat_texcoord_scale );
	df = 0.30 * dispColor.r + 0.59 * dispColor.g + 0.11 * dispColor.b;
#endif
	return df;
}

vec4 getTransformedPosition() {
#if defined(HAS_TEX_DISPLACEMENT)
	return getTransformedPositionAlongNormal(getVertexDisplacement() * mat_displacement_strength);
#else
	return position;
#endif
}

void sendVaryings(in vec4 apos) {
	v_worldNormal = normalize(mat3(modelMatrix) * normal.xyz);
//	v_texcoord = (textureMatrix*vec4(texcoord.x*mat_texcoord_scale.x,texcoord.y*mat_texcoord_scale.y,0,1)).xy;
	v_texcoord = getTransformedTexcoord();
	v_worldPosition = (modelMatrix * apos).xyz;

	#if HAS_COLOR
		v_color = color;
	#endif
}

%mainVertex%

)";


static const string shader_pbr_main_vert = R"(
void main (void){
	vec4 npos = getTransformedPosition();
	sendVaryings(npos);
	gl_Position = modelViewProjectionMatrix * npos;
}
)";
