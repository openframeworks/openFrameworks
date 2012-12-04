
#pragma once

#include "ofMain.h"
#include "aiMesh.h"

class ofxAssimpMeshHelper {

public:

	ofxAssimpMeshHelper() {
		mesh = NULL;
        blendMode = OF_BLENDMODE_ALPHA;
        twoSided = false;
        hasChanged = false;
        validCache = false;
	}
    
    aiMesh * mesh; // pointer to the aiMesh we represent.

    ofVbo vbo;
    
    ofTexture texture;
    vector<ofIndexType> indices;
    
    ofMaterial material;
    
    ofBlendMode blendMode;
    
    bool twoSided;
    bool hasChanged;

    vector<aiVector3D> animatedPos;
    vector<aiVector3D> animatedNorm;

    ofMesh cachedMesh;
    bool validCache;
};
