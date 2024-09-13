//
//  ofxAssimpTexture.h
//
//  Created by Lukasz Karluk on 25/03/13.
//
//

#pragma once
#include "ofTexture.h"
#include <assimp/material.h>
#include "ofMaterial.h"

namespace ofx::assimp {
class Texture {
public:
	static std::unordered_map< int, ofMaterialTextureType > sAiTexTypeToOfTexTypeMap;
	
	//	void setup(const ofTexture & texture, const of::filesystem::path & texturePath, bool bTexRepeat = true);
	void setup(const of::filesystem::path & texturePath, bool bTexRepeat = true);
	
	ofTexture & getTextureRef();
	of::filesystem::path getTexturePath();
	bool hasTexture();
	
	void setAiTextureType(aiTextureType aTexType);
	aiTextureType getAiTextureType() const;
	std::string getAiTextureTypeAsString() const;
	
	ofMaterialTextureType getOfTextureType();
	std::string getOfTextureTypeAsString();
	
	static ofMaterialTextureType ofTextureTypeForAiType(aiTextureType aTexType);
	static aiTextureType aiTextureTypeForOfType(ofMaterialTextureType aTexType);
	
private:
	static void _initTextureTypeMap();
	
	ofTexture texture;
	of::filesystem::path texturePath;
	aiTextureType textureType = aiTextureType_NONE;
	std::string mTexTypeStr;
	ofMaterialTextureType mOfTexType = OF_MATERIAL_TEXTURE_DIFFUSE;
};
}
