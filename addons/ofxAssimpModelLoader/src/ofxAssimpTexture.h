//
//  ofxAssimpTexture.h
//
//  Created by Lukasz Karluk on 25/03/13.
//
//

#pragma once

#include "ofTexture.h"
#include <assimp/material.h>

class ofxAssimpTexture {

public:

	void setup(const ofTexture & texture, const of::filesystem::path & texturePath, bool bTexRepeat = true);

	ofTexture & getTextureRef();
	of::filesystem::path getTexturePath();
	bool hasTexture();

	void setTextureType(aiTextureType aTexType);
	aiTextureType getTextureType() const;
	std::string getTextureTypeAsString() const;

private:

	ofTexture texture;
	of::filesystem::path texturePath;
	aiTextureType textureType;
	std::string mTexTypeStr;
};
