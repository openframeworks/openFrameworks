//
//  ofxAsssimpTexture.cpp
//
//  Created by Lukasz Karluk on 27/03/13.
//
//

#include "ofxAssimpTexture.h"

ofxAssimpTexture::ofxAssimpTexture() {
    this->texture = NULL;
    this->texturePath = "";
}

ofxAssimpTexture::ofxAssimpTexture(ofxAssimpTexture * assimpTexture) {
    if(assimpTexture == NULL) {
        this->texture = NULL;
        this->texturePath = "";
        return;
    }
    
    this->texture = assimpTexture->getTexturePtr();
    this->texturePath = assimpTexture->getTexturePath();
}

ofxAssimpTexture::ofxAssimpTexture(ofTexture * texture, string texturePath) {
    this->texture = texture;
    this->texturePath = texturePath;
}

ofTexture * ofxAssimpTexture::getTexturePtr() {
    return texture;
}

string ofxAssimpTexture::getTexturePath() {
    return texturePath;
}

bool ofxAssimpTexture::hasTexture() {
    return (texture != NULL);
}