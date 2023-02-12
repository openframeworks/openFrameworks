//
//  ofxAssimpMeshHelper.h
//  Created by Lukasz Karluk on 4/12/12.
//

#pragma once

#include "ofMaterial.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ofxAssimpTexture.h"
#include "ofVbo.h"
#include "ofMesh.h"
#include "glm/mat4x4.hpp"

struct aiMesh;

class ofxAssimpMeshHelper {

public:

	void addTexture(ofxAssimpTexture & aAssimpTex);
	bool hasTexture(aiTextureType aTexType = aiTextureType_DIFFUSE);

	ofTexture & getTextureRef(aiTextureType aTexType = aiTextureType_DIFFUSE);
	std::vector<std::shared_ptr<ofxAssimpTexture>> & getAllMeshTextures(){ return meshTextures; }

	aiMesh * mesh = NULL; // pointer to the aiMesh we represent.

	ofVbo vbo;

	std::vector<ofIndexType> indices;

	ofMaterial material;

	ofBlendMode blendMode = OF_BLENDMODE_ALPHA;

	bool twoSided = false;
	bool hasChanged = false;

	std::vector<aiVector3D> animatedPos;
	std::vector<aiVector3D> animatedNorm;

	//diffuse texture - legacy api
	ofxAssimpTexture assimpTexture;

	ofMesh cachedMesh;
	bool validCache = false;

	glm::mat4 matrix;

protected:
	//for normal, specular, etc - we include the diffuse too with a null deleter
	std::vector <std::shared_ptr<ofxAssimpTexture>> meshTextures;

};
