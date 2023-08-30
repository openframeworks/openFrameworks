OF_GLSL_SHADER_HEADER


// -- output to frag shader ------------- //
out vec2 v_texcoord; // pass the texCoord if needed
out vec3 v_transformedNormal;
out vec3 v_normal;
out vec3 v_eyePosition;
out vec3 v_worldPosition;
out vec4 v_color;

out vec4 mv_positionVarying;
// ------------------------------ //


//-- set automatically by OF ---------//
// these are passed in from OF programmable renderer
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 textureMatrix;
uniform vec4 globalColor;
uniform mat4 normalMatrix;

uniform float usingTexture;
uniform float usingColors;

in vec4         position;
in vec4         normal;
//in vec4         color;
in vec2         texcoord;
//----------------------------------

uniform sampler2D mapDiffuse;
uniform sampler2D mapNormal;
uniform sampler2D mapDisplacement;
uniform sampler2D mapInfluence;


vec2 textureRepeatTimes = vec2(1.0, 1.0);

void main (void){
	vec2 newTexcoord = mod(texcoord * textureRepeatTimes, 1.0);
	newTexcoord = texcoord * textureRepeatTimes;
	vec4 bumpColor = texture(mapDisplacement, newTexcoord );
	float df = 0.30 * bumpColor.r + 0.59 * bumpColor.g + 0.11 * bumpColor.b;
	
	vec4 newVertexPos = vec4(normal.xyz * df * float(0.5), 0.0);
	float influence = texture(mapInfluence, newTexcoord).r;
	newVertexPos = newVertexPos * influence;
	newVertexPos += position;
	
	
	vec4 eyePosition = modelViewMatrix * newVertexPos;
	mv_positionVarying = eyePosition;
	
	vec3 tempNormal = (normalMatrix * normal).xyz;
	v_transformedNormal = normalize(tempNormal);
	v_normal = normal.xyz;
	v_eyePosition = (eyePosition.xyz) / eyePosition.w;
	v_worldPosition = (modelMatrix * position).xyz;

	v_texcoord = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
	
	
	v_color = globalColor;
	
	gl_Position = modelViewProjectionMatrix * newVertexPos;
	
}
