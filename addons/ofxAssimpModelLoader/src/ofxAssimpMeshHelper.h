//
//  ofxAssimpMeshHelper.h
//  Created by Lukasz Karluk on 4/12/12.
//

#pragma once

#include "ofMain.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ofxAssimpTexture.h"

struct aiMesh;

class ofxAssimpMeshHelper {

public:

	ofxAssimpMeshHelper();
    
    bool hasTexture();
    ofTexture & getTextureRef();
    
    aiMesh * mesh; // pointer to the aiMesh we represent.

    ofVbo vbo;
    
    ofxAssimpTexture assimpTexture;
    std::vector<ofIndexType> indices;
    
    ofMaterial material;
    
    ofBlendMode blendMode;
    
    bool twoSided;
    bool hasChanged;

    std::vector<aiVector3D> animatedPos;
    std::vector<aiVector3D> animatedNorm;

    ofMesh cachedMesh;
    bool validCache;
    
    ofMatrix4x4 matrix;
};
