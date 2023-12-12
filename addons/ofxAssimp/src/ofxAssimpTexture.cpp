//
//  ofxAsssimpTexture.cpp
//
//  Created by Lukasz Karluk on 27/03/13.
//
//

#include "ofxAssimpTexture.h"
#include "ofLog.h"
#include "ofUtils.h"

using namespace ofx::assimp;

std::unordered_map< int, ofMaterialTextureType > ofx::assimp::Texture::sAiTexTypeToOfTexTypeMap;

//-------------------------------------------
void ofx::assimp::Texture::setup(const of::filesystem::path & texturePath, bool bTexRepeat) {
//	this->texture = texture;
	if( bTexRepeat ){
		this->texture.setTextureWrap(GL_REPEAT, GL_REPEAT);
	}else{
		this->texture.setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	}
	this->texturePath = texturePath;
}

#ifndef TARGET_WIN32
//this is a hack to allow for weak definations of functions that might not exist in older assimp versions
const char *aiTextureTypeToString(enum aiTextureType in)__attribute__((weak));
#endif

//-------------------------------------------
void ofx::assimp::Texture::setAiTextureType(aiTextureType aTexType){
	textureType = aTexType;

	if( textureType >= 0 && textureType < AI_TEXTURE_TYPE_MAX){
		if(aiTextureTypeToString){
			mTexTypeStr = aiTextureTypeToString(getAiTextureType());
		}else{
			mTexTypeStr = "textureType:"+ofToString(getAiTextureType());
		}
	}else{
		ofLogError("ofxAssimpTexture::setTextureType") << ": unknown aiTextureType type " << aTexType;
		mTexTypeStr = "NONE";
	}
}

//-------------------------------------------
ofTexture & Texture::getTextureRef() {
	return texture;
}

//-------------------------------------------
of::filesystem::path Texture::getTexturePath() {
	return texturePath;
}

//-------------------------------------------
bool Texture::hasTexture() {
	return texture.isAllocated();
}

//-------------------------------------------
aiTextureType Texture::getAiTextureType() const{
	return textureType;
}

//-------------------------------------------
std::string Texture::getAiTextureTypeAsString() const{
	return mTexTypeStr;
}

//-------------------------------------------
ofMaterialTextureType Texture::getOfTextureType() {
	return ofTextureTypeForAiType(textureType);
}

//-------------------------------------------
std::string Texture::getOfTextureTypeAsString() {
	return ofMaterial::getTextureTypeAsString(getOfTextureType());
}

//-------------------------------------------
ofMaterialTextureType Texture::ofTextureTypeForAiType(aiTextureType aTexType) {
	_initTextureTypeMap();
	if( sAiTexTypeToOfTexTypeMap.count(aTexType) > 0 ) {
		return sAiTexTypeToOfTexTypeMap[(int)aTexType];
	}
	return OF_MATERIAL_TEXTURE_NONE;
}

//-------------------------------------------
aiTextureType Texture::aiTextureTypeForOfType(ofMaterialTextureType aTexType) {
	_initTextureTypeMap();
	for( auto iter = sAiTexTypeToOfTexTypeMap.begin(); iter != sAiTexTypeToOfTexTypeMap.end(); iter++ ) {
		if( iter->second == aTexType ) {
			return (aiTextureType)iter->first;
		}
	}
	return aiTextureType_NONE;
}

//-------------------------------------------
void Texture::_initTextureTypeMap() {
	if(sAiTexTypeToOfTexTypeMap.size() < 1 ) {
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_DIFFUSE] = OF_MATERIAL_TEXTURE_DIFFUSE;
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_EMISSIVE] = OF_MATERIAL_TEXTURE_EMISSIVE;
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_NORMALS] = OF_MATERIAL_TEXTURE_NORMAL;
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_LIGHTMAP] = OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC;
		// aiTextureType_SHININESS // probably should be used with phong, but going to map to roughness
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_SHININESS] = OF_MATERIAL_TEXTURE_ROUGHNESS;
		//17 = aiTextureType_AMBIENT_OCCLUSION; //use this when we want to support newer assimp only
		sAiTexTypeToOfTexTypeMap[17] = OF_MATERIAL_TEXTURE_OCCLUSION;
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_AMBIENT] = OF_MATERIAL_TEXTURE_AMBIENT;
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_SPECULAR] = OF_MATERIAL_TEXTURE_SPECULAR;
		sAiTexTypeToOfTexTypeMap[15] = OF_MATERIAL_TEXTURE_METALLIC; //aiTextureType_METALNESS
		sAiTexTypeToOfTexTypeMap[16] = OF_MATERIAL_TEXTURE_ROUGHNESS; //aiTextureType_DIFFUSE_ROUGHNESS
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_DISPLACEMENT] = OF_MATERIAL_TEXTURE_DISPLACEMENT;
		sAiTexTypeToOfTexTypeMap[20] = OF_MATERIAL_TEXTURE_CLEARCOAT; // aiTextureType_CLEARCOAT = 20,
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_NONE] = OF_MATERIAL_TEXTURE_NONE;
	}
}


