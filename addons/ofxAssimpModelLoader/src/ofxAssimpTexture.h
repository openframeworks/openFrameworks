//
//  ofxAssimpTexture.h
//
//  Created by Lukasz Karluk on 25/03/13.
//
//

#pragma once

#include "ofConstants.h"
#include "ofTexture.h"

class ofxAssimpTexture {

public:
    
    ofxAssimpTexture();
	ofxAssimpTexture(ofTexture texture, std::string texturePath);

    ofTexture & getTextureRef();
	std::string getTexturePath();
    bool hasTexture();
    
private:
    
    ofTexture texture;
	std::string texturePath;
    
};
