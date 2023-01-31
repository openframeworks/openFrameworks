//
//  ofxAssimpTexture.h
//
//  Created by Lukasz Karluk on 25/03/13.
//
//

#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include <assimp/material.h>

class ofxAssimpTexture {

public:

	void setup(const ofTexture & texture, std::string texturePath, bool bTexRepeat = true);

    ofTexture & getTextureRef();
	std::string getTexturePath();
    bool hasTexture();
    
    void setTextureType(aiTextureType aTexType);
    aiTextureType getTextureType() const;
    std::string getTextureTypeAsString() const;
    
private:
    
    ofTexture texture;
	std::string texturePath;
    aiTextureType textureType;
    std::string mTexTypeStr;
};
