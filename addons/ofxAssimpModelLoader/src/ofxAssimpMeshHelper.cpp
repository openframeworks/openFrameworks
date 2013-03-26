//
//  ofxAssimpMeshHelper.cpp
//  Created by Lukasz Karluk on 4/12/12.
//

#include "ofxAssimpMeshHelper.h"
#include "ofxAssimpUtils.h"
#include "aiMesh.h"

ofxAssimpMeshHelper::ofxAssimpMeshHelper() {
    mesh = NULL;
    assimpTexture = NULL;
    blendMode = OF_BLENDMODE_ALPHA;
    twoSided = false;
    hasChanged = false;
    validCache = false;
}

ofxAssimpMeshHelper::~ofxAssimpMeshHelper() {
    //
}

bool ofxAssimpMeshHelper::hasTexture() {
    if(assimpTexture != NULL) {
        return assimpTexture->hasTexture();
    }
    return false;
}

ofTexture * ofxAssimpMeshHelper::getTexturePtr() {
    if(hasTexture()) {
        return assimpTexture->getTexturePtr();
    }
    return NULL;
}
