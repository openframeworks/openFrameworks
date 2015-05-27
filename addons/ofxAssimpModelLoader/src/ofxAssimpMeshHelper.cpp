//
//  ofxAssimpMeshHelper.cpp
//  Created by Lukasz Karluk on 4/12/12.
//

#include "ofxAssimpMeshHelper.h"
#include "ofxAssimpUtils.h"

ofxAssimpMeshHelper::ofxAssimpMeshHelper() {
    mesh = NULL;
    blendMode = OF_BLENDMODE_ALPHA;
    twoSided = false;
    hasChanged = false;
    validCache = false;
}

bool ofxAssimpMeshHelper::hasTexture() {
    return assimpTexture.hasTexture();
}

ofTexture & ofxAssimpMeshHelper::getTextureRef() {
	return assimpTexture.getTextureRef();
}
