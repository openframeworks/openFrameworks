static const string shader_utils = R"(

#ifndef TWO_PI
#define TWO_PI 6.2831853072
#endif

#ifndef PI
#define PI 3.14159265359
#endif

#ifndef HALF_PI
#define HALF_PI 1.57079632679
#endif

#ifndef PHI
#define PHI 0.5*(1.0+sqrt(5.0))
#endif

#ifndef ONE_OVER_PI
#define ONE_OVER_PI 1.0/PI
#endif



#ifndef FUNCTIONS_UTILS
#define FUNCTIONS_UTILS

#ifndef MEDIUMP_FLT_MAX
#define MEDIUMP_FLT_MAX    65504.0
#endif

#ifndef saturateMediump
#define saturateMediump(x) min(x, MEDIUMP_FLT_MAX)
#endif

#ifndef FLT_EPS
#define FLT_EPS            0.00006103515625
#endif

#ifndef saturate
#define saturate(x) clamp(x, 0.0, 1.0)
#endif



// pow functions //
float pow5( in float a ) {
	float aSq = a * a;
	return aSq * aSq * a;
}

vec2 pow5( in vec2 a ) {
	vec2 aSq = a * a;
	return aSq * aSq * a;
}

vec3 pow5( in vec3 a ) {
	vec3 aSq = a * a;
	return aSq * aSq * a;
}

vec4 pow5( in vec4 a ) {
	vec4 aSq = a * a;
	return aSq * aSq * a;
}

#endif


#ifndef FUNCTIONS_COLOR_SPACES
#define FUNCTIONS_COLOR_SPACES

// https://github.com/patriciogonzalezvivo/lygia/blob/main/color/space/gamma2linear.glsl
#if !defined(GAMMA_CONST) && !defined(PBR_QUALITY_LEVEL_LOW)
#define GAMMA_CONST 2.2
#define ONE_OVER_GAMMA 1.0/GAMMA_CONST
#endif

float gamma2Linear(in float v) {
#ifdef GAMMA_CONST
	return pow(v, GAMMA_CONST);
#else
	// assume gamma 2.0
	return v * v;
#endif
}

vec3 gamma2Linear( in vec3 v ) {
#ifdef GAMMA_CONST
	return pow(v, vec3(GAMMA_CONST));
#else
	// assume gamma 2.0
	return v * v;
#endif
}

vec4 gamma2Linear(in vec4 v) {
	return vec4(gamma2Linear(v.rgb), v.a);
}

float linear2Gamma(in float v) {
#ifdef GAMMA_CONST
	return pow(v, ONE_OVER_GAMMA);
#else
	return sqrt(v);
#endif
}

vec3 linear2Gamma(in vec3 v) {
#ifdef GAMMA_CONST
	return pow(v, vec3(ONE_OVER_GAMMA));
#else
	return sqrt(v);
#endif
}

vec4 linear2Gamma(in vec4 v) {
	return vec4(linear2Gamma(v.rgb), v.a);
}

#endif


#ifndef FUNCTIONS_NORMALS
#define FUNCTIONS_NORMALS
//-- PerturbNormal normal map code from ------------------------------------ //
// http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/
// http://www.thetenthplanet.de/archives/1180
mat3 CotangentFrame(in vec3 N, in vec3 p, in vec2 uv) {
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

vec3 PerturbNormal(in vec3 normalMap, in vec3 N, in vec3 V, in vec2 texcoord) {
	// assume N, the interpolated vertex normal and
	// V, the view vector (vertex to eye)
	vec3 map = normalMap * 255. / 127. - 128. / 127.;
	mat3 TBN = CotangentFrame(N, -V, texcoord);
	return normalize(TBN * map);
}


// https://github.com/mrdoob/three.js/blob/dev/src/renderers/shaders/ShaderChunk/bumpmap_pars_fragment.glsl.js
// Bump Mapping Unparametrized Surfaces on the GPU by Morten S. Mikkelsen
// https://mmikk.github.io/papers3d/mm_sfgrad_bump.pdf

// Evaluate the derivative of the height w.r.t. screen-space using forward differencing (listing 2)
vec2 dHdxy_fwd(in SAMPLER normalMap, in vec2 uv, in float hStrength) {
	
	vec2 dSTdx = dFdx( v_texcoord ).xy;
	vec2 dSTdy = dFdy( v_texcoord ).xy;
	
	float Hll = hStrength * TEXTURE(normalMap, uv ).x;
	float dBx = hStrength * TEXTURE(normalMap, uv + dSTdx ).x - Hll;
	float dBy = hStrength * TEXTURE(normalMap, uv + dSTdy ).x - Hll;
	
	return vec2( dBx, dBy );
	
}

vec3 perturbNormalArb( vec3 apos, vec3 anorm, vec2 dHdxy, float faceDirection ) {
	vec3 vSigmaX = dFdx( apos.xyz );
	vec3 vSigmaY = dFdy( apos.xyz );
	vec3 vN = anorm; // normalized
	
	vec3 R1 = cross( vSigmaY, vN );
	vec3 R2 = cross( vN, vSigmaX );
	
	float fDet = dot( vSigmaX, R1 ) * faceDirection;
	
	vec3 vGrad = sign( fDet ) * ( dHdxy.x * R1 + dHdxy.y * R2 );
	return normalize( abs( fDet ) * anorm - vGrad );
	
}
#endif

)";
