//
//  CubeMapShaders.h
//  CubeMap
//
//  Created by Nick Hardeman on 10/21/22.
//

#pragma once
#include "ofConstants.h"

#ifndef STRINGIFY
#define STRINGIFY(A) #A
#endif

class ofCubeMapShaders {
public:
	
	struct ShaderSource {
		std::string vertShader;
		std::string fragShader;
	};
	
	static std::string defaultVertShader() {
		std::string vshader = "#version 150\n";
		vshader += STRINGIFY(
								in vec4 position;
								
								uniform mat4 uProjection;
								uniform mat4 uView;
								
								out vec3 oLocalPos;
								
								void main() {
									oLocalPos = position.xyz;
									gl_Position = uProjection * uView * vec4(position.xyz, 1.0);
								}
								);
		return vshader;
	}
	
	static ShaderSource equiRectToCubeMap() {
		ShaderSource rsource;
		rsource.vertShader = defaultVertShader();
		
		rsource.fragShader = "#version 150\n";
		rsource.fragShader += STRINGIFY(
							 out vec4 FRAGCOLOR;\n
							 in vec3 oLocalPos;\n
							 
							 uniform float uFlipY;\n
							 
							 uniform sampler2D uEquirectangularTex;\n
							 
							 // https://learnopengl.com/PBR/IBL/Diffuse-irradiance \n
							 const vec2 invAtan = vec2(0.1591, 0.3183);\n
							 vec2 SampleSphericalMap(vec3 v) {\n
								 vec2 uv = vec2(atan(v.z, v.x), asin(v.y));\n
								 uv *= invAtan;\n
								 uv += 0.5;\n
								 if( uFlipY > 0.5 ) {\n
									 uv.y = 1.0-uv.y;\n
								 }\n
								 return uv;\n
							 }
							 
							 void main() {
								 vec2 uv = SampleSphericalMap(normalize(oLocalPos)); // make sure to normalize localPos
								 vec3 color = texture(uEquirectangularTex, uv).rgb;
								 
								 FRAGCOLOR = vec4(color, 1.0);
							 }
							 );
		return rsource;
	}
	
	static ShaderSource irriadianceCubeMap() {
		ShaderSource rsource;
		rsource.vertShader = defaultVertShader();
		
		rsource.fragShader = "#version 150\n";
		rsource.fragShader += STRINGIFY(
							 out vec4 FRAGCOLOR;\n
							 in vec3 oLocalPos;\n
							 
							 //uniform float uFlipY;\n
							 
							 uniform samplerCube environmentMap;\n
							 const float PI = 3.14159265359;\n
							 
							 // https://learnopengl.com/PBR/IBL/Diffuse-irradiance \n
							 void main() {\n
								 // the sample direction equals the hemisphere's orientation\n
								 // The world vector acts as the normal of a tangent surface\n
								 // from the origin, aligned to WorldPos. Given this normal, calculate all\n
								 // incoming radiance of the environment. The result of this radiance\n
								 // is the radiance of light coming from -Normal direction, which is what\n
								 // we use in the PBR shader to sample irradiance.\n
								 vec3 N = normalize(oLocalPos);\n
								 
								 vec3 irradiance = vec3(0.0);\n
								 
								 // tangent space calculation from origin point\n
								 vec3 up    = vec3(0.0, 1.0, 0.0);\n
								 vec3 right = normalize(cross(up, N));\n
								 up         = normalize(cross(N, right));\n
								 
								 float sampleDelta = 0.025;\n
								 float nrSamples = 0.0;\n
								 for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) {\n
									 for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) {\n
										 // spherical to cartesian (in tangent space)\n
										 vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));\n
										 // tangent space to world\n
										 vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;\n
										 
										 irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);\n
										 nrSamples++;\n
									 }\n
								 }\n
								 irradiance = PI * irradiance * (1.0 / float(nrSamples));\n
								 
								 FRAGCOLOR = vec4(irradiance, 1.0);\n
							 }
							 );
		return rsource;
	}
	
	static ShaderSource renderShader() {
		ShaderSource rsource;
		rsource.vertShader = "#version 150\n";
		rsource.vertShader += STRINGIFY(
							 in vec4 position;\n
							 // set from OF //\n
							 uniform mat4 modelViewProjectionMatrix;
							 uniform mat4 projectionMatrix;\n
							 uniform mat4 viewMatrix;\n
							 \n
							 out vec3 oTexCoords;\n
							 \n
							 void main() {\n
								 oTexCoords = position.xyz;\n
								 // removing the translation of the view matrix\n
								 vec4 pos = projectionMatrix * mat4(mat3(viewMatrix)) * position;\n
								 gl_Position = pos.xyww;\n
								 //							 gl_Position = modelViewProjectionMatrix * position;
							 }\n
							 );
		rsource.fragShader = "#version 150\n";
		rsource.fragShader += STRINGIFY(
							 in vec3 oTexCoords;\n
							 
							 uniform samplerCube uCubeMap;\n
							 uniform float uIsHDR;\n
							 uniform float uExposure = 1.0;\n
							 uniform float uRoughness = 0.0;\n
							 uniform float uMaxMips = 1;\n
							 
							 out vec4 FRAGCOLOR;\n
										
							vec3 getEnvColor( in vec3 aR, in float aroughness ) {\n
								float lod = aroughness * (uMaxMips-1);\n
								vec3 ai = textureLod(uCubeMap, aR, floor(lod) ).rgb;\n
								vec3 ab = textureLod(uCubeMap, aR, clamp(ceil(lod), floor(lod), uMaxMips-1) ).rgb;\n
								return mix(ai, ab, lod-floor(lod) );\n
							}\n
							 
							 void main() {\n
								 vec3 envColor = vec3(0.0);\n
								 if( uMaxMips > 1 && uRoughness > 0.0 ) {\n
									 envColor = getEnvColor(oTexCoords, uRoughness );\n
//									envColor = textureLod(uCubeMap, oTexCoords, getLodRoughness(uRoughness*uRoughness) ).rgb;\n
//									 envColor = texture(uCubeMap, oTexCoords, getLodRoughness(uRoughness) ).rgb;\n
								 } else {\n
									envColor = texture(uCubeMap, oTexCoords).rgb; \n
//									 envColor = textureLod(uCubeMap, oTexCoords, 0.0).rgb;\n
								 }\n
//								 envColor = textureLod(uCubeMap, oTexCoords, 0.0).rgb;\n
								 
								 if( uIsHDR > 0.5 ) {\n
									 
									 // exposure tone mapping\n
									 vec3 mapped = vec3(1.0) - exp(-envColor * uExposure);\n
									 // gamma correction\n
									 envColor = pow(mapped, vec3(1.0 / 2.2));\n
									 
									 //envColor = envColor / (envColor + vec3(1.0));\n
									 //envColor = pow(envColor, vec3(1.0/2.2));\n
								 }\n
								 
								 FRAGCOLOR = vec4(envColor, 1.0);\n
							 }\n
							 
							 );
		
		return rsource;
	}
	
	static std::string hammersley() {
		std::string ssrc = STRINGIFY(
									 // ----------------------------------------------------------------------------
									 // http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
									 // efficient VanDerCorpus calculation.
									#ifndef TARGET_OPENGLES
									 float RadicalInverse_VdC(uint bits) {
										 bits = (bits << 16u) | (bits >> 16u);
										 bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
										 bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
										 bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
										 bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
										 return float(bits) * 2.3283064365386963e-10; // / 0x100000000
									 }
									 
									 // ----------------------------------------------------------------------------
									 vec2 HammersleyBits(uint i, uint N){
										 return vec2(float(i)/float(N), RadicalInverse_VdC(i));
									 }
									#else
									 
									 float VanDerCorput(uint n, uint base) {
										 float invBase = 1.0 / float(base);
										 float denom   = 1.0;
										 float result  = 0.0;
										 
										 for(uint i = 0u; i < 32u; ++i){
											 if(n > 0u){
												 denom   = mod(float(n), 2.0);
												 result += denom * invBase;
												 invBase = invBase / 2.0;
												 n       = uint(float(n) / 2.0);
											 }
										 }
										 return result;
									 }
									 // ----------------------------------------------------------------------------
									 vec2 HammersleyNoBitOps(uint i, uint N){
										 return vec2(float(i)/float(N), VanDerCorput(i, 2u));
									 }
									#endif
									 
									 
									 // ----------------------------------------------------------------------------
									 vec2 Hammersley(uint i, uint N){
										#ifdef TARGET_OPENGLES
										 return HammersleyNoBitOps(i, N);
										#else
										 return HammersleyBits(i, N);
										#endif
									 }
									 );
		return ssrc;
	}
	
	// https://learnopengl.com/PBR/IBL/Specular-IBL
	static ShaderSource prefilter() {
		ShaderSource rsource;
		rsource.vertShader = defaultVertShader();
		
		rsource.fragShader = "#version 150\n";
		rsource.fragShader += STRINGIFY(
										out vec4 FragColor;
										in vec3 oLocalPos;
										
										uniform samplerCube environmentMap;
										uniform float roughness;
										
										const float PI = 3.14159265359;
										
										);
		
		rsource.fragShader += hammersley();
		
		rsource.fragShader += STRINGIFY(
										// ----------------------------------------------------------------------------
										float DistributionGGX(vec3 N, vec3 H, float roughness) {
											float a = roughness*roughness;
											float a2 = a*a;
											float NdotH = max(dot(N, H), 0.0);
											float NdotH2 = NdotH*NdotH;
											
											float nom   = a2;
											float denom = (NdotH2 * (a2 - 1.0) + 1.0);
											denom = PI * denom * denom;
											
											return nom / denom;
										}
										
										// ----------------------------------------------------------------------------
										vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness){
											float a = roughness*roughness;
											
											float phi = 2.0 * PI * Xi.x;
											float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
											float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
											
											// from spherical coordinates to cartesian coordinates - halfway vector
											vec3 H;
											H.x = cos(phi) * sinTheta;
											H.y = sin(phi) * sinTheta;
											H.z = cosTheta;
											
											// from tangent-space H vector to world-space sample vector
											vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
											vec3 tangent   = normalize(cross(up, N));
											vec3 bitangent = cross(N, tangent);
											
											vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
											return normalize(sampleVec);
										}
										// ----------------------------------------------------------------------------
										void main(){
											vec3 N = normalize(oLocalPos);
											
											// make the simplyfying assumption that V equals R equals the normal
											vec3 R = N;
											vec3 V = R;
											
											const uint SAMPLE_COUNT = 1024u;
											vec3 prefilteredColor = vec3(0.0);
											float totalWeight = 0.0;
											
											for(uint i = 0u; i < SAMPLE_COUNT; ++i){
												// generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
												vec2 Xi = Hammersley(i, SAMPLE_COUNT);
												vec3 H = ImportanceSampleGGX(Xi, N, roughness);
												vec3 L  = normalize(2.0 * dot(V, H) * H - V);
												
												float NdotL = max(dot(N, L), 0.0);
												if(NdotL > 0.0){
													// sample from the environment's mip level based on roughness/pdf
													float D   = DistributionGGX(N, H, roughness);
													float NdotH = max(dot(N, H), 0.0);
													float HdotV = max(dot(H, V), 0.0);
													float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;
													
													float resolution = 512.0; // resolution of source cubemap (per face)
													float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
													float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);
													
													float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
													
													prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
													totalWeight      += NdotL;
												}
											}
											
											prefilteredColor = prefilteredColor / totalWeight;
											//prefilteredColor = vec3(1.0,0.0,0.0);
											FragColor = vec4(prefilteredColor, 1.0);
										}
										
										);
		
		return rsource;
	}
	
	
	static ShaderSource brdfLUT() {
		ShaderSource rsource;
		rsource.vertShader = "#version 150\n";
		rsource.vertShader += STRINGIFY(
										in vec4 position;
										in vec2 texcoord;
										
										uniform mat4 modelViewProjectionMatrix;
										
										out vec2 TexCoords;
										
										void main() {
											TexCoords = texcoord;
											gl_Position = modelViewProjectionMatrix * position;
										}
										);
		rsource.fragShader = "#version 150\n";
		rsource.fragShader += STRINGIFY(
										out vec4 FragColor;
										in vec2 TexCoords;
										
										const float PI = 3.14159265359;
										
										);
		
		rsource.fragShader += hammersley();
		
		rsource.fragShader += STRINGIFY(
										// ----------------------------------------------------------------------------
										vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness){
											float a = roughness*roughness;
											
											float phi = 2.0 * PI * Xi.x;
											float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
											float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
											
											// from spherical coordinates to cartesian coordinates - halfway vector
											vec3 H;
											H.x = cos(phi) * sinTheta;
											H.y = sin(phi) * sinTheta;
											H.z = cosTheta;
											
											// from tangent-space H vector to world-space sample vector
											vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
											vec3 tangent   = normalize(cross(up, N));
											vec3 bitangent = cross(N, tangent);
											
											vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
											return normalize(sampleVec);
										}
										// ----------------------------------------------------------------------------
										float GeometrySchlickGGX(float NdotV, float roughness){
											// note that we use a different k for IBL
											float a = roughness;
											float k = (a * a) / 2.0;
											
											float nom   = NdotV;
											float denom = NdotV * (1.0 - k) + k;
											
											return nom / denom;
										}
										// ----------------------------------------------------------------------------
										float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
											float NdotV = max(dot(N, V), 0.0);
											float NdotL = max(dot(N, L), 0.0);
											float ggx2 = GeometrySchlickGGX(NdotV, roughness);
											float ggx1 = GeometrySchlickGGX(NdotL, roughness);
											
											return ggx1 * ggx2;
										}
										// ----------------------------------------------------------------------------
										vec2 IntegrateBRDF(float NdotV, float roughness){
											vec3 V;
											V.x = sqrt(1.0 - NdotV*NdotV);
											V.y = 0.0;
											V.z = NdotV;
											
											float A = 0.0;
											float B = 0.0;
											
											vec3 N = vec3(0.0, 0.0, 1.0);
											
											const uint SAMPLE_COUNT = 1024u;
											for(uint i = 0u; i < SAMPLE_COUNT; ++i){
												// generates a sample vector that's biased towards the
												// preferred alignment direction (importance sampling).
												vec2 Xi = Hammersley(i, SAMPLE_COUNT);
												vec3 H = ImportanceSampleGGX(Xi, N, roughness);
												vec3 L = normalize(2.0 * dot(V, H) * H - V);
												
												float NdotL = max(L.z, 0.0);
												float NdotH = max(H.z, 0.0);
												float VdotH = max(dot(V, H), 0.0);
												
												if(NdotL > 0.0)
												{
													float G = GeometrySmith(N, V, L, roughness);
													float G_Vis = (G * VdotH) / (NdotH * NdotV);
													float Fc = pow(1.0 - VdotH, 5.0);
													
													A += (1.0 - Fc) * G_Vis;
													B += Fc * G_Vis;
												}
											}
											A /= float(SAMPLE_COUNT);
											B /= float(SAMPLE_COUNT);
											return vec2(A, B);
										}
										// ----------------------------------------------------------------------------
										void main(){
											vec2 integratedBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y);
											FragColor.rg = integratedBRDF;
											FragColor.b = 0;
//											FragColor.r = 1.0;
//											FragColor.g = 0.0;
											FragColor.a = 1.0;
										}
										);
		return rsource;
	}
    
};
