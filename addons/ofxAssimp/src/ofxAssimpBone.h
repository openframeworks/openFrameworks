//
//  ofxAssimpBone.h
//  ofxAssimpExample
//
//  Created by Nick Hardeman on 10/24/23.
//

#pragma once
#include "ofxAssimpNode.h"
#include "ofxAssimpSrcBone.h"

class ofVboMesh;

namespace ofx::assimp {
class Bone : public ofx::assimp::Node {
public:
	
	virtual NodeType getType() override { return OFX_ASSIMP_BONE; }
	
	void setSrcBone( std::shared_ptr<ofx::assimp::SrcBone> aSrcBone );
	
//	virtual void updateFromSrcBone();
	void cacheGlobalBoneMat(glm::mat4& aInvMat);
	virtual void draw(float aAxisSize=30.0f);
	
	std::shared_ptr<ofx::assimp::SrcBone> getSrcBone() { return mSrcBone; }
	
//	std::shared_ptr<ofx::assimp::Bone> getBone( const std::string& aName );
//	void findBoneRecursive( const std::string& aName, std::shared_ptr<Bone>& returnBone );
	
//	aiMatrix4x4& getAiMatrix() { return mAiMatrix; }
//	aiMatrix4x4& getAiMatrixGlobal() { return mAiMatrixGlobal; }
	aiMatrix4x4& getAiOffsetMatrix() { return mOffsetMatrix;}
	const glm::mat4& getCachedGlobalBoneMat() { return mCachedGlobalBoneMat; }
	const aiMatrix4x4& getAiCachedGlobalBoneMat() { return mAiCachedGlobalBoneMat; }
	
protected:
	std::vector< std::shared_ptr<ofx::assimp::Bone> > mChildBones;
	std::shared_ptr<ofx::assimp::SrcBone> mSrcBone;
	
	void _initRenderMesh();
	
	glm::mat4 mCachedGlobalBoneMat = glm::mat4(1.0f);
//	glm::quat mOffsetOrientation;
	
	aiMatrix4x4 mAiMatrix, mAiCachedGlobalBoneMat;
//	aiMatrix4x4 mAiMatrixGlobal;
	aiMatrix4x4 mOffsetMatrix;
	glm::mat4 mGlmOffsetMat;
	
	// 0: +x
	// 1: +y
	// 2: +z
	// 3: -x
	// 4: -y
	// 5: -z
	int mAlignAxis = -1;
	static std::unordered_map< int, std::shared_ptr<ofVboMesh> > sRenderMeshes;
//	static std::shared_ptr<ofVboMesh> sRenderMesh;
};
}
