//
//  ofxAsssimpTexture.cpp
//
//  Created by Lukasz Karluk on 27/03/13.
//
//

#include "ofxAssimpTexture.h"
#include "ofLog.h"
#include "ofUtils.h"

void ofxAssimpTexture::setup(const ofTexture & texture, const of::filesystem::path & texturePath, bool bTexRepeat) {
	this->texture = texture;
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

void ofxAssimpTexture::setTextureType(aiTextureType aTexType){
	textureType = aTexType;

	if( textureType >= 0 && textureType < AI_TEXTURE_TYPE_MAX){
		if(aiTextureTypeToString){
			mTexTypeStr = aiTextureTypeToString(getTextureType());
		}else{
			mTexTypeStr = "textureType:"+ofToString(getTextureType());
		}
	}else{
		ofLogError("ofxAssimpTexture::setTextureType") << ": unknown aiTextureType type " << aTexType;
		mTexTypeStr = "NONE";
	}
}

ofTexture & ofxAssimpTexture::getTextureRef() {
	return texture;
}

of::filesystem::path ofxAssimpTexture::getTexturePath() {
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
