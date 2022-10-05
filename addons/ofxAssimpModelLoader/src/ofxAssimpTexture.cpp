//
//  ofxAsssimpTexture.cpp
//
//  Created by Lukasz Karluk on 27/03/13.
//
//

#include "ofxAssimpTexture.h"
#include "ofConstants.h"
#include "ofLog.h"

using namespace std;

void ofxAssimpTexture::setup(const ofTexture & texture, string texturePath, bool bTexRepeat) {
    this->texture = texture;
    if( bTexRepeat ){
		this->texture.setTextureWrap(GL_REPEAT, GL_REPEAT);
    }else{
		this->texture.setTextureWrap(GL_CLAMP, GL_CLAMP);
	}
    this->texturePath = texturePath;
}

void ofxAssimpTexture::setTextureType(aiTextureType aTexType){
	textureType = aTexType;
	
	if( textureType >= 0 && textureType < AI_TEXTURE_TYPE_MAX){
		mTexTypeStr = aiTextureTypeToString(getTextureType());
    }else{
		ofLogError("ofxAssimpTexture::setTextureType") << ": unknown aiTextureType type " << aTexType;
		mTexTypeStr = "NONE";
	}
}

ofTexture & ofxAssimpTexture::getTextureRef() {
    return texture;
}

string ofxAssimpTexture::getTexturePath() {
    return texturePath;
}

bool ofxAssimpTexture::hasTexture() {
    return texture.isAllocated();
}

aiTextureType ofxAssimpTexture::getTextureType() const{
	return textureType;
}

std::string ofxAssimpTexture::getTextureTypeAsString() const{
	return mTexTypeStr;
}
