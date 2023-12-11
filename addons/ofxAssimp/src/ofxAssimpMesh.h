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
#include "ofxAssimpNode.h"
#include "ofxAssimpBounds.h"
#include <unordered_map>
#include "ofxAssimpBone.h"
#include "ofxAssimpSrcMesh.h"

struct aiMesh;
namespace ofx::assimp {

class Mesh : public ofx::assimp::Node {
public:
	
	virtual NodeType getType() override { return OFX_ASSIMP_MESH; }
	
	bool hasTexture();
	bool hasTexture( ofMaterialTextureType aType );
	
	std::size_t getNumTextures();
	ofTexture& getTexture();
	ofTexture& getTexture(ofMaterialTextureType aType);
	std::shared_ptr<ofx::assimp::Texture> getTexture( unsigned int aindex );
	
	std::size_t getNumVertices();
	bool hasNormals();
	aiMesh* getAiMesh();
	std::size_t getNumIndices();
	
	void setSrcMesh( std::shared_ptr<ofx::assimp::SrcMesh> aSrcMesh );
	
	ofMesh& getStaticMesh();
	ofMesh& getMesh();
	
	ofMesh getTransformedStaticMesh();
	ofMesh getTransformedMesh();
	
	Bounds getLocalBounds();
	Bounds getModelBounds();
	Bounds getGlobalBounds();
	
	void recalculateBounds(bool abForce=false);
	
	std::shared_ptr<ofMaterial> material;
	std::shared_ptr<ofVbo> vbo;
	
	ofBlendMode blendMode = OF_BLENDMODE_ALPHA;
	bool twoSided = false;
	
	bool hasChanged = false;
	bool validCache = false;
	bool bConvertedToLeftHand = true;
	
	std::vector<aiVector3D> animatedVertices;
	std::vector<aiVector3D> animatedNormals;
	
	std::vector< std::shared_ptr<ofx::assimp::Bone> > mBones;
	
	std::shared_ptr<ofx::assimp::SrcMesh> getSrcMesh() { return mSrcMesh; }
	
protected:
	virtual void onPositionChanged() override;
	virtual void onOrientationChanged() override;
	virtual void onScaleChanged() override;
	
	std::shared_ptr<ofx::assimp::SrcMesh> mSrcMesh;
	
	// calculate the local bounds for all of the vertices
	// has no position, scale or rotations applied
	// to account for all animated frames
	Bounds mLocalBounds;
	// bounds relative to local translation, rotation and scale
	Bounds mModelBounds;
	// global bounds, should include all transformations
	Bounds mGlobalBounds;
	// should be updated when any transformations are changed or vertices, bones, etc.
	bool bBoundsDirty = false;
	
		
	ofMesh mMesh;
	ofMesh mAnimatedMesh;
	bool mBInitedAnimatedMesh = false;
	
};
}
