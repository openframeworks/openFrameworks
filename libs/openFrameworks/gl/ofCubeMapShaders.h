//
//  CubeMapShaders.h
//  CubeMap
//
//  Created by Nick Hardeman on 10/21/22.
//

#pragma once

#include "ofGLUtils.h"
#include "ofConstants.h"

class ofCubeMapShaders {
public:
	
	struct ShaderSource {
		std::string vertShader;
		std::string fragShader;
	};
	
	static std::string getGLSLHeader() {
		std::string vstr = "#version 150\n";
		if( ofGetGLRenderer() ) {
			vstr = "#version "+ofGLSLVersionFromGL(ofGetGLRenderer()->getGLVersionMajor(), ofGetGLRenderer()->getGLVersionMinor())+"\n";
		}
		#ifdef TARGET_OPENGLES
		vstr += "#define TARGET_OPENGLES\n";
		//vstr += "#extension GL_OES_standard_derivatives : enable\n";
		vstr += "precision highp float;\n";
		vstr += "precision highp int;\n";
		vstr += "precision highp sampler2D;\n";
		vstr += "precision highp samplerCube;\n";
		#endif
		return vstr;
	}
	
	static std::string defaultVertShader() {
		std::string vshader = getGLSLHeader();
		vshader += R"(in vec4 position;
					uniform mat4 uProjection;
								uniform mat4 uView;
								
								out vec3 oLocalPos;
								
								void main() {
									oLocalPos = position.xyz;
									gl_Position = uProjection * uView * vec4(position.xyz, 1.0);
								}
					)";
								
		return vshader;
	}
	
	static ShaderSource equiRectToCubeMap() {
		ShaderSource rsource;
		rsource.vertShader = defaultVertShader();
		
		rsource.fragShader = getGLSLHeader();
		rsource.fragShader += R"(
							 out vec4 FRAGCOLOR;
							 in vec3 oLocalPos;
							 
							 uniform float uFlipY;
							uniform float uConvertToNonFloat; // 0.0 // convert hdr to non float texture
							 
							 uniform sampler2D uEquirectangularTex;
							 
							 // https://learnopengl.com/PBR/IBL/Diffuse-irradiance
							 const vec2 invAtan = vec2(0.1591, 0.3183);
							 vec2 SampleSphericalMap(vec3 v) {
								 vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
								 uv *= invAtan;
								 uv += 0.5;
								 if( uFlipY > 0.5 ) {
									 uv.y = 1.0-uv.y;
								 }
								 return uv;
							 }
							 
							 void main() {
								 vec2 uv = SampleSphericalMap(normalize(oLocalPos)); // make sure to normalize localPos
								 vec3 color = texture(uEquirectangularTex, uv).rgb;
						
								if(uConvertToNonFloat > 0.5) {
									// hdr tone mapping
									color = color / (color+vec3(1.0));
									// gamma correction 
									//color = pow(color, vec3(1.0 / 2.2));
									color = clamp( color, 0.0, 1.0);
								}
								 
								 FRAGCOLOR = vec4(color, 1.0);
							 }
							 )";
		return rsource;
	}
	
	static ShaderSource irriadianceCubeMap() {
		ShaderSource rsource;
		rsource.vertShader = defaultVertShader();
		
		rsource.fragShader = getGLSLHeader();
		rsource.fragShader += R"(
							 out vec4 FRAGCOLOR;
							 in vec3 oLocalPos;
							 							 
							 uniform samplerCube environmentMap;
							 const float PI = 3.14159265359;
							 
							 // https://learnopengl.com/PBR/IBL/Diffuse-irradiance
							 void main() {
								 // the sample direction equals the hemisphere's orientation
								 // The world vector acts as the normal of a tangent surface
								 // from the origin, aligned to WorldPos. Given this normal, calculate all
								 // incoming radiance of the environment. The result of this radiance
								 // is the radiance of light coming from -Normal direction, which is what
								 // we use in the PBR shader to sample irradiance.
								 vec3 N = normalize(oLocalPos);
								 
								 vec3 irradiance = vec3(0.0);
								 
								 // tangent space calculation from origin point
								 vec3 up    = vec3(0.0, 1.0, 0.0);
								 vec3 right = normalize(cross(up, N));
								 up         = normalize(cross(N, right));
								 
								 float sampleDelta = 0.025;
								 float nrSamples = 0.0;
								 for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) {
									 for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) {
										 // spherical to cartesian (in tangent space)
										 vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
										 // tangent space to world
										 vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
										 
										 irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
										 //nrSamples++;
										   nrSamples += 1.0;
									 }
								 }
								 //irradiance = PI * irradiance * (1.0 / float(nrSamples));
								 irradiance = irradiance * PI * (1.0 / max(nrSamples, 1.0));
								 FRAGCOLOR = vec4(irradiance, 1.0);
							 }
							 )";
		return rsource;
	}
	
	static ShaderSource renderShader() {
		ShaderSource rsource;
		rsource.vertShader = getGLSLHeader();
		rsource.vertShader += R"(
							 in vec4 position;
							 // set from OF //
							 uniform mat4 modelViewProjectionMatrix;
							 uniform mat4 projectionMatrix;
							 uniform mat4 viewMatrix;
							 
							 out vec3 oTexCoords;
							 
							 void main() {
								 oTexCoords = position.xyz;
								 // removing the translation of the view matrix
								 vec4 pos = projectionMatrix * mat4(mat3(viewMatrix)) * position;
								 gl_Position = pos.xyww;
								 //							 gl_Position = modelViewProjectionMatrix * position;
							 }
							 )";
		rsource.fragShader = getGLSLHeader();
		rsource.fragShader += R"(
							 in vec3 oTexCoords;
							 
							uniform samplerCube uCubeMap;
							uniform float uIsHDR;
							uniform float uExposure;// = 1.0;
							uniform float uRoughness;// = 0.0;
							uniform float uMaxMips;// = 1;
							 
							out vec4 FRAGCOLOR;
										
							vec3 getEnvColor( in vec3 aR, in float aroughness ) {
								float lod = aroughness * (uMaxMips-1.0);
								vec3 ai = textureLod(uCubeMap, aR, floor(lod) ).rgb;
								vec3 ab = textureLod(uCubeMap, aR, clamp(ceil(lod), floor(lod), uMaxMips-1.0) ).rgb;
								return mix(ai, ab, lod-floor(lod) );
							}
							 
							 void main() {
								 vec3 envColor = vec3(0.0);
								 if( uMaxMips > 1.0 && uRoughness > 0.0 ) {
									 envColor = getEnvColor(oTexCoords, uRoughness );
								 } else {
									envColor = texture(uCubeMap, oTexCoords).rgb;
								 }
		
								 if( uIsHDR > 0.5 ) {
									 //envColor = envColor / (envColor + vec3(1.0));
									 // exposure tone mapping
									 vec3 mapped = vec3(1.0) - exp(-envColor * uExposure);
									
									 // gamma correction
									 envColor = pow(mapped, vec3(1.0 / 2.2));
											 
									 //envColor = envColor / (envColor + vec3(1.0));
									 //envColor = pow(envColor, vec3(1.0/2.2));
								 } else {
									// gamma correction
									envColor = pow(envColor, vec3(1.0 / max(uExposure*2.2, 0.1) ) );
								}
		
								 FRAGCOLOR = vec4(clamp(envColor, 0.0, 1.0), 1.0);
							 }
							 
							 )";
		
		return rsource;
	}
	
	static std::string hammersley() {
		std::string ssrc = R"(
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
									 )";
		return ssrc;
	}
	
	// https://learnopengl.com/PBR/IBL/Specular-IBL
	static ShaderSource prefilter() {
		ShaderSource rsource;
		rsource.vertShader = defaultVertShader();
		
		rsource.fragShader = getGLSLHeader();
		rsource.fragShader += R"(
										out vec4 FragColor;
										in vec3 oLocalPos;
										
										uniform samplerCube environmentMap;
										uniform float uroughness;
										uniform float resolution; // resolution of source cubemap (per face)
										
										const float PI = 3.14159265359;
										
										)";
		
		rsource.fragShader += hammersley();
		
		rsource.fragShader += R"(
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
											
											//float troughness = uroughness;
											
											// make the simplyfying assumption that V equals R equals the normal
											vec3 R = N;
											vec3 V = R;
											#ifdef TARGET_OPENGLES
											const uint SAMPLE_COUNT = 1024u;
											#else
											const uint SAMPLE_COUNT = 2048u;
											#endif
											vec3 prefilteredColor = vec3(0.0);
											float totalWeight = 0.0;
											
											for(uint i = 0u; i < SAMPLE_COUNT; ++i){
												// generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
												vec2 Xi = Hammersley(i, SAMPLE_COUNT);
												vec3 H = ImportanceSampleGGX(Xi, N, uroughness);
												vec3 L  = normalize(2.0 * dot(V, H) * H - V);
												
												float NdotL = max(dot(N, L), 0.0);
												if(NdotL > 0.0){
													// sample from the environment's mip level based on roughness/pdf
													float D   = DistributionGGX(N, H, uroughness);
													float NdotH = max(dot(N, H), 0.0);
													float HdotV = max(dot(H, V), 0.0);
													float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;
													
//													//float resolution = 512.0; // resolution of source cubemap (per face)
													float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
													float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);
													
													float mipLevel = uroughness == 0.0 ? 0.0 : max(0.5 * log2(saSample / saTexel), 0.0);
													//mipLevel = clamp( mipLevel, 0.0, 4.0);
													
													prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
													totalWeight      += NdotL;
												}
											}
											if(totalWeight < 0.0001 ) {
												totalWeight = 0.0001;
											}
											
											prefilteredColor = prefilteredColor / totalWeight;
											FragColor = vec4(prefilteredColor, 1.0);
										}
										
										)";
		
		return rsource;
	}
	
	
	static ShaderSource brdfLUT() {
		ShaderSource rsource;
		rsource.vertShader = getGLSLHeader();
		rsource.vertShader += R"(
										in vec4 position;
										in vec2 texcoord;
										
										uniform mat4 modelViewProjectionMatrix;
										
										out vec2 TexCoords;
										
										void main() {
											TexCoords = texcoord;
											gl_Position = modelViewProjectionMatrix * position;
										}
										)";
		rsource.fragShader = getGLSLHeader();
		rsource.fragShader += R"(
										out vec4 FragColor;
										in vec2 TexCoords;
										
										const float PI = 3.14159265359;
										
										)";
		
		rsource.fragShader += hammersley();
		
		rsource.fragShader += R"(
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
										)";
		return rsource;
	}
    
};
