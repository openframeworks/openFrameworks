//
//  ofxAssimpTexture.h
//
//  Created by Lukasz Karluk on 25/03/13.
//
//

#pragma once

#include "ofMain.h"

class ofxAssimpTexture {

public:
    
    ofxAssimpTexture();
    ofxAssimpTexture(ofxAssimpTexture * assimpTexture);
    ofxAssimpTexture(ofTexture * texture, string texturePath);

    ofTexture * getTexturePtr();
    string getTexturePath();
    bool hasTexture();
    
private:
    
    ofTexture * texture;
    string texturePath;
    
};