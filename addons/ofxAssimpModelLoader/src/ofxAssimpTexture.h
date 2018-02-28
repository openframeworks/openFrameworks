//
//  ofxAssimpTexture.h
//
//  Created by Lukasz Karluk on 25/03/13.
//
//

#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofMain.h"

class ofxAssimpTexture {

public:
    
    ofxAssimpTexture();
	ofxAssimpTexture(ofTexture texture, std::string texturePath);
	ofxAssimpTexture(const ofBuffer &texData, std::string texturePath);

    ofTexture & getTextureRef();
	std::string getTexturePath();
    bool hasTexture();
	bool isLoaded();

	bool loadTextureFromTextureData();
	bool reloadTextureFromTextureData();

private:
    
    ofTexture texture;
	std::string texturePath;
	ofPixels *textureData;
	bool loaded;
	bool bTextureDataLoaded;

};
