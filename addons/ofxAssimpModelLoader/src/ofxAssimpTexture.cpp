//
//  ofxAsssimpTexture.cpp
//
//  Created by Lukasz Karluk on 27/03/13.
//
//

#include "ofxAssimpTexture.h"
#include "ofConstants.h"

using namespace std;

ofxAssimpTexture::ofxAssimpTexture() {
    this->texturePath = "";
    this->loaded = false;
    this->textureData = NULL;
}

//ofxAssimpTexture::ofxAssimpTexture(ofxAssimpTexture &assimpTexture) {
//    if(!assimpTexture.hasTexture()) {
//        this->texturePath = "";
//        this->loaded = false;
//        this->textureData = NULL;
//        return;
//    }
//
//    this->texture = assimpTexture.getTextureRef();
//    this->texturePath = assimpTexture.getTexturePath();
//}

ofxAssimpTexture::ofxAssimpTexture(ofTexture texture, string texturePath) {
    this->texture = texture;
    this->texturePath = texturePath;
    this->loaded = true;
}

ofxAssimpTexture::ofxAssimpTexture(const ofBuffer &texData, string texturePath) {
    this->texturePath = texturePath;
    this->loaded = false;
    this->textureData = new ofPixels();
    this->bTextureDataLoaded = ofLoadImage(*(this->textureData), texData);
}

ofxAssimpTexture::~ofxAssimpTexture(){
    if (this->textureData != NULL && this->textureData->isAllocated()) {
        this->textureData->clear();
        delete this->textureData;
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

bool ofxAssimpTexture::isLoaded() {
    return loaded;
}

bool ofxAssimpTexture::loadTextureFromTextureData() {
    if (loaded || !bTextureDataLoaded) {
        return false;
    }

    this->texture.allocate(*(this->textureData));
    this->texture.loadData(*(this->textureData));
    return true;
}

bool ofxAssimpTexture::reloadTextureFromTextureData() {
    if (loaded) {
        this->texture.clear();
    }
    if (!bTextureDataLoaded) {
        return false;
    }
    this->texture.allocate(*(this->textureData));
    this->texture.loadData(*(this->textureData));
    return true;
}