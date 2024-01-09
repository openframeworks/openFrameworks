OF_GLSL_SHADER_HEADER

// we are going to use 2 lights
// we also have 2 ofLights in ofApp
#define MAX_LIGHTS 2

uniform sampler2D mapDiffuse;
uniform sampler2D mapNormal;
uniform sampler2D mapDisplacement;
uniform sampler2D mapAORoughMetal;
uniform sampler2D mapInfluence;

uniform float uIsDrawingInto;

//-- coming in from vertex shader ------//
in vec2 v_texcoord;
in vec3 v_transformedNormal;
in vec3 v_normal;
in vec3 v_eyePosition;
in vec3 v_worldPosition;
in vec4 v_color;

in vec4 mv_positionVarying;
//---------------------------------------


//-- Material uniforms -----------------//
uniform vec4 mat_ambient;
uniform vec4 mat_diffuse;
uniform vec4 mat_specular;
uniform vec4 mat_emissive;
uniform float mat_shininess;
uniform vec4 global_ambient;
//---------------------------------------

// -- copied from libs / openFrameworks / gl / shaders --------------- //
// set from ofMaterial 
struct lightData
{
	float enabled;
	vec4 ambient;
	float type; // 0 = pointlight 1 = directionlight
	vec4 position; // where are we
	vec4 diffuse; // how diffuse
	vec4 specular; // what kinda specular stuff we got going on?
				   // attenuation
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	// only for spot
	float spotCutoff;
	float spotCosCutoff;
	float spotExponent;
	// spot and area
	vec3 spotDirection;
	// only for directional
	vec3 halfVector;
	// only for area
	float width;
	float height;
	vec3 right;
	vec3 up;
};

uniform lightData lights[MAX_LIGHTS];


void pointLight( in lightData light, in vec3 normal, in vec3 ecPosition3, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular ){
	float nDotVP;       // normal . light direction
	float nDotHV;       // normal . light half vector
	float pf;           // power factor
	float attenuation;  // computed attenuation factor
	float d;            // distance from surface to light source
	vec3  VP;           // direction from surface to light position
	vec3  halfVector;   // direction of maximum highlights
	vec3 eye = -normalize(v_eyePosition);
	
	// Compute vector from surface to light position
	VP = vec3 (light.position.xyz) - ecPosition3;
	
	// Compute distance between surface and light position
	d = length(VP);
	
	
	// Compute attenuation
	attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);
	
	// Normalize the vector from surface to light position
	VP = normalize(VP);
	halfVector = normalize(VP + eye);
	nDotHV = max(0.0, dot(normal, halfVector));
	nDotVP = max(0.0, dot(normal, VP));
	
	ambient += light.ambient.rgb * attenuation;
	diffuse += light.diffuse.rgb * nDotVP * attenuation;
#ifndef TARGET_OPENGLES
#define SPECULAR_REFLECTION
#endif
#ifndef SPECULAR_REFLECTION
	// ha! no branching :)
	pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));
	specular += light.specular.rgb * pf * nDotVP * attenuation;
#else
	// fresnel factor
	// http://en.wikibooks.org/wiki/GLSL_Programming/Unity/Specular_Highlights_at_Silhouettes
	float w = pow(1.0 - max(0.0, dot(halfVector, VP)), 5.0);
	vec3 specularReflection = attenuation * vec3(light.specular.rgb)
	* mix(vec3(mat_specular.rgb), vec3(1.0), w)
	* pow(nDotHV, mat_shininess);
	specular += mix(vec3(0.0), specularReflection, step(0.0000001, nDotVP));
#endif
}
//--------------------------------------- //

// final color to output
out vec4 FRAGCOLOR;

vec2 textureRepeatTimes = vec2(1.0, 1.0);


//-- normal ------------------------------------ //
// http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/
// http://www.thetenthplanet.de/archives/1180
mat3 CotangentFrame(vec3 N, vec3 p, vec2 uv) {
	// get edge vectors of the pixel triangle
	vec3 dp1 = dFdx(p);
	vec3 dp2 = dFdy(p);
	vec2 duv1 = dFdx(uv);
	vec2 duv2 = dFdy(uv);
	
	// solve the linear system
	vec3 dp2perp = cross(dp2, N);
	vec3 dp1perp = cross(N, dp1);
	vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
	
	// construct a scale-invariant frame
	float invmax = inversesqrt(max(dot(T, T), dot(B, B)));
	return mat3(T * invmax, B * invmax, N);
}

vec3 PerturbNormal(vec3 normalMap, vec3 N, vec3 V, vec2 texcoord) {
	// assume N, the interpolated vertex normal and
	// V, the view vector (vertex to eye)
	vec3 map = normalMap * 255. / 127. - 128. / 127.;
	mat3 TBN = CotangentFrame(N, -V, texcoord);
	return normalize(TBN * map);
}
//----------------------------------------------------//

void main() {
	vec2 newTexcoord = v_texcoord * textureRepeatTimes;
	
	vec3 mv_normal = normalize(v_transformedNormal);
	vec3 normalMapVec = texture(mapNormal, newTexcoord).xyz;
	vec3 PN = PerturbNormal( normalMapVec, mv_normal, v_eyePosition, newTexcoord);
	
	float gamma = 2.2;
	
	vec3 diffuseFromTex = texture(mapDiffuse, newTexcoord).rgb;
	vec3 diffuseColor = pow(diffuseFromTex, vec3(gamma));
	
	vec3 baseColor = diffuseColor;
	
	vec3 ambient = global_ambient.rgb;
	vec3 diffuse = vec3(0.0,0.0,0.0);
	vec3 specular = vec3(0.0,0.0,0.0);
	
	vec3 normal = normalize(PN);
	
	// loop through the lights and accumulate output into ambient, diffuse and specular values
	for( int i = 0; i < MAX_LIGHTS; i++ ){
		if(lights[i].enabled<0.5) continue;
		if(lights[i].type<0.5){
			pointLight(lights[i], normal, v_eyePosition, ambient, diffuse, specular);
		}
	}
	
	////////////////////////////////////////////////////////////
	// now add the material info
	vec4 tex = texture(mapDiffuse, newTexcoord);
	vec4 localColor = vec4(ambient,1.0) * mat_ambient.a * tex + vec4(diffuse,1.0) * mat_diffuse.a * tex + (vec4(specular,1.0) * mat_specular)*mat_specular.a;
	
	vec3 color = localColor.rgb;
	// ambient occlusion values are baked into the r value of the mapAORoughMetal material
	vec3 aoRoughMetal = texture( mapAORoughMetal, newTexcoord).xyz;
	color *= (0.35 + aoRoughMetal.r * 0.65);
	
	diffuse = vec3(0.0);
	specular = vec3(0.0);
	// lets recalc light for smooth surface
	for( int i = 0; i < MAX_LIGHTS; i++ ){
		if(lights[i].enabled<0.5) continue;
		if(lights[i].type<0.5){
			pointLight(lights[i], mv_normal, v_eyePosition, ambient, diffuse, specular);
		}
	}
	
	vec4 bumpColor = texture(mapDisplacement, newTexcoord );
	vec3 innerColor = mat_emissive.rgb;
	innerColor = diffuse * innerColor + (specular.rgb * mat_specular.a).rgb;
	innerColor = innerColor * (1.0-mat_emissive.a) + mat_emissive.rgb * mat_emissive.a;
	
	vec4 mapInfValue = texture(mapInfluence, newTexcoord);
	
	color = mix( innerColor, color, pow(mapInfValue.r, 0.5) * pow(clamp(bumpColor.r*2.0, 0.0, 1.0), 0.3) );
	
	float mixAmnt = mapInfValue.g;
	color.rgb = mix(color.rgb, vec3(1,1,1), mixAmnt * uIsDrawingInto );
	
	FRAGCOLOR = vec4(color, 1.0);
}







