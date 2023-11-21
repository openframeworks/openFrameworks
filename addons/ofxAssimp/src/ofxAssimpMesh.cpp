//
//  ofxAssimpMeshHelper.cpp
//  Created by Lukasz Karluk on 4/12/12.
//

#include "ofxAssimpMesh.h"
#include "ofxAssimpUtils.h"
#include "of3dGraphics.h"
#include "ofGraphics.h"

using std::make_shared;
using std::shared_ptr;

using namespace ofx::assimp;

//-------------------------------------------
bool Mesh::hasTexture() {
	if(mSrcMesh) {
		return mSrcMesh->hasTexture();
	}
	ofLogWarning("ofx::assimp::Mesh::getTexture") << "SrcMesh is not set.";
	return false;
}

//-------------------------------------------
bool Mesh::hasTexture(ofMaterialTextureType aType){
	if(mSrcMesh) {
		return mSrcMesh->hasTexture(aType);
	}
	ofLogWarning("ofx::assimp::Mesh::getTexture") << "SrcMesh is not set.";
	return false;
}

//-------------------------------------------
std::size_t Mesh::getNumTextures() {
	if(mSrcMesh) {
		return mSrcMesh->getNumTextures();
	}
	ofLogWarning("ofx::assimp::Mesh::getTexture") << "SrcMesh is not set.";
	return 0;
}

//-------------------------------------------
ofTexture& Mesh::getTexture() {
	if(mSrcMesh) {
		return mSrcMesh->getTexture();
	}
	ofLogWarning("ofx::assimp::Mesh::getTexture") << "SrcMesh is not set.";
	return ofx::assimp::SrcMesh::sDummyTex;
}

//-------------------------------------------
ofTexture& Mesh::getTexture(ofMaterialTextureType aType){
	if(mSrcMesh) {
		return mSrcMesh->getTexture( aType );
	}
	ofLogWarning("ofx::assimp::Mesh::getTexture") << "SrcMesh is not set.";
	return ofx::assimp::SrcMesh::sDummyTex;
}

//-------------------------------------------
std::shared_ptr<ofx::assimp::Texture> Mesh::getTexture( unsigned int aindex ) {
	if(mSrcMesh) {
		if( aindex < 0 || aindex >= getNumTextures() ) {
			ofLogWarning("ofx::assimp::Mesh::getTexture") << "out of bounds: " << aindex << " - " << getNumTextures();
		}
		return mSrcMesh->getAllMeshTextures()[aindex];
	}
	ofLogWarning("ofx::assimp::Mesh::getTexture") << "SrcMesh is not set.";
	return std::shared_ptr<ofx::assimp::Texture>();
}

//-------------------------------------------
size_t Mesh::getNumVertices() {
	if(mSrcMesh && mSrcMesh->getAiMesh()) {
		return mSrcMesh->getAiMesh()->mNumVertices;
	}
	return 0;
}

//-------------------------------------------
bool Mesh::hasNormals() {
	if(mSrcMesh && mSrcMesh->getAiMesh()) {
		return mSrcMesh->getAiMesh()->HasNormals();
	}
	return false;
}

//-------------------------------------------
aiMesh* Mesh::getAiMesh() {
	if(mSrcMesh) {
		return mSrcMesh->getAiMesh();
	}
	return nullptr;
}

//-------------------------------------------
std::size_t Mesh::getNumIndices() {
	if(mSrcMesh) {
		return mSrcMesh->indices.size();
	}
	return 0;
}

//-------------------------------------------
void Mesh::setSrcMesh( std::shared_ptr<ofx::assimp::SrcMesh> aSrcMesh ) {
	mSrcMesh = aSrcMesh;
	
	// if we have bones, no offset, since we are controlled by the bones //
//	if( mSrcMesh->getAiMesh() && mSrcMesh->getAiMesh()->mNumBones < 1 ) {
//		setOfNodeFromAiMatrix(mSrcMesh->getAiNode()->mTransformation, this );
//	}
	
	if( mSrcMesh ) {
		material = mSrcMesh->material;
		twoSided = mSrcMesh->twoSided;
		blendMode = mSrcMesh->blendMode;
		vbo = make_shared<ofVbo>();
		mSrcMesh->setupVbo(vbo);
//		ofVbo temp = *mSrcMesh->vbo;
//		vbo = std::make_shared<ofVbo>(temp);
//		vbo = make_shared<ofVbo>();
		
		mLocalBounds = mSrcMesh->getLocalBounds();
		bConvertedToLeftHand = mSrcMesh->bConvertedToLeftHand;
	}
}

//-------------------------------------------
ofMesh& Mesh::getStaticMesh() {
	if( mSrcMesh && mSrcMesh->getAiMesh() && mMesh.getNumVertices() < 1 ) {
		if( hasTexture() ) {
			aiMeshToOfMesh(mSrcMesh->getAiMesh(), mMesh, !bConvertedToLeftHand, &getTexture() );
		} else {
			aiMeshToOfMesh(mSrcMesh->getAiMesh(), mMesh, !bConvertedToLeftHand, nullptr);
		}
	}
	return mMesh;
}

//-------------------------------------------
ofMesh& Mesh::getMesh() {
	if( animatedVertices.size() > 0 ) {
		if(!validCache){
			if( !mBInitedAnimatedMesh ) {
				mBInitedAnimatedMesh=true;
				if(mSrcMesh && mSrcMesh->getAiMesh()) {
					if( hasTexture() ) {
						aiMeshToOfMesh(mSrcMesh->getAiMesh(), mAnimatedMesh, !bConvertedToLeftHand, &getTexture() );
					} else {
						aiMeshToOfMesh(mSrcMesh->getAiMesh(), mAnimatedMesh, !bConvertedToLeftHand, nullptr);
					}
				}
			}
			
			mAnimatedMesh.clearVertices();
			mAnimatedMesh.clearNormals();
			mAnimatedMesh.addVertices(aiVecVecToOfVecVec(animatedVertices));
//			mAnimatedMesh.addVertices(animatedVertices);
			if(animatedNormals.size() == animatedVertices.size() ) {
				mAnimatedMesh.addNormals(aiVecVecToOfVecVec(animatedNormals));
//				mAnimatedMesh.addNormals(animatedNormals);
			}
			validCache = true;
		}
		return mAnimatedMesh;
	}
	return getStaticMesh();
}

//-------------------------------------------
ofMesh Mesh::getTransformedStaticMesh() {
	auto srcMesh = getStaticMesh();
	glm::mat4 gmat = getGlobalTransformMatrix();
	glm::mat4 normalMat = glm::transpose(glm::inverse(gmat));
	bool bNormals = srcMesh.usingNormals() && (srcMesh.getNumNormals() == srcMesh.getNumVertices());
	for( auto& vert : srcMesh.getVertices() ) {
		vert = gmat * glm::vec4(vert.x, vert.y, vert.z, 1.0f );
	}
	if( bNormals ) {
		for( auto& normal : srcMesh.getNormals() ) {
			normal = normalMat * glm::vec4(normal.x, normal.y, normal.z, 1.0f );
		}
	}
	return srcMesh;
}

//-------------------------------------------
ofMesh Mesh::getTransformedMesh() {
	auto srcMesh = getMesh();
	glm::mat4 gmat = getGlobalTransformMatrix();
	glm::mat4 normalMat = glm::transpose(glm::inverse(gmat));
	bool bNormals = srcMesh.usingNormals() && (srcMesh.getNumNormals() == srcMesh.getNumVertices());
	for( auto& vert : srcMesh.getVertices() ) {
		vert = gmat * glm::vec4(vert.x, vert.y, vert.z, 1.0f );
	}
	if( bNormals ) {
		for( auto& normal : srcMesh.getNormals() ) {
			normal = normalMat * glm::vec4(normal.x, normal.y, normal.z, 1.0f );
		}
	}
	return srcMesh;
}

//-------------------------------------------
Bounds Mesh::getLocalBounds() {
	recalculateBounds();
	return mLocalBounds;
}

//-------------------------------------------
Bounds Mesh::getModelBounds() {
	recalculateBounds();
	return mModelBounds;
}

//-------------------------------------------
Bounds Mesh::getGlobalBounds() {
	recalculateBounds();
	return mGlobalBounds;
}

//-------------------------------------------
void Mesh::recalculateBounds(bool abForce) {
	if(hasChanged || abForce) {
		// calculate local bounds since verts have changed //
		mLocalBounds.clear();
//		mLocalBounds.include( cachedMesh.getVertices() );
		if( animatedVertices.size() > 3 ) {
			mLocalBounds.include(animatedVertices);
		} else {
//			mLocalBounds.include( getStaticMesh().getVertices() );
			if( mSrcMesh ) {
				mLocalBounds = mSrcMesh->getLocalBounds();
			} else {
				mLocalBounds.include( getStaticMesh().getVertices() );
			}
		}
		bBoundsDirty = true;
	}
	if(bBoundsDirty) {
		// update the model bounds
		mModelBounds.clear();
		const glm::mat4& modelMat = getLocalTransformMatrix();
		auto& tverts = mLocalBounds.getBoundingVerts();//{mLocalBounds.min, mLocalBounds.max};
		mModelBounds.include(modelMat, tverts );
		// update globally
		mGlobalBounds.clear();
		mGlobalBounds.include(getGlobalTransformMatrix(), tverts );
	}
	bBoundsDirty=false;
}

//-------------------------------------------
void Mesh::onPositionChanged() {
	bBoundsDirty = true;
}

//-------------------------------------------
void Mesh::onOrientationChanged() {
	bBoundsDirty = true;
}

//-------------------------------------------
void Mesh::onScaleChanged() {
	bBoundsDirty = true;
}
