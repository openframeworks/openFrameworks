#include "ofxAssimpSrcBone.h"
#include "ofGraphics.h"
#include "of3dGraphics.h"
#include "of3dUtils.h"
#include "ofxAssimpUtils.h"

using namespace ofx::assimp;

//--------------------------------------------------------------
void SrcBone::setAiBone(aiBone* aAiBone, aiNode* aAiNode) {
	SrcNode::setAiNode(aAiNode);
	// we do not sure a pointer to the aiBone because they are different than nodes
	// in the scene????
	// aiMeshes store the bone weights etc in aiBone. Multiple aiBones can reference the same aiNode
	// this class is storing references to the aiNode that happens to be a bone
//	mAiBone = aAiBone;
	if(aAiBone) {
		mOffsetMatrix = aAiBone->mOffsetMatrix;
	} else {
		std::cout << "SrcBone::setAiBone : aiBone is NULL - " << getName() << std::endl;
	}
	mAiBone = aAiBone;
	
	mAiMatrix = mAiNode->mTransformation;
}

////--------------------------------------------------------------
//void SrcBone::update() {
//	if( mAiNode == nullptr ) {
//		ofLogWarning("SrcBone::update mAiNode nullptr!!!");
//		return;
//	}
//
////	if( mAiBone == nullptr ) {
////		ofLogWarning("SrcBone::update mAiBone nullptr!!!");
////		return;
////	}
//
////	if( bRoot ) {
////		std::cout << "Bone: " << getName() << " has parent: " << (getParent() != nullptr) << " | " << ofGetFrameNum() << std::endl;
////	}
//	// update //
////	mAiMatrix = mAiBone->mOffsetMatrix;
//	mAiMatrix = mAiNode->mTransformation;
//
////	mAiMatrixGlobal = mAiNode->mTransformation;
////	mBoneLocalTransform = glmMat4ToAiMatrix4x4(mAiMatrix);
//	// and now append all node transformations down the parent chain until we're back at mesh coordinates again
////	if( bRoot ) {
////		const aiNode* tempNode = mAiNode;
////		aiNode* node = scene->mRootNode->FindNode(mAiBone->mName);
////		const aiNode* tempNode = mAiNode->mParent;
////		while(tempNode) {
//////			aiMatrix4x4 m = tempNode->mTransformation;
//////			m.Transpose();
////		// check your matrix multiplication order here!!!
//////			mAiMatrix = m * mAiMatrix;//boneMatrices[a];
//////			ofLogNotice("SrcBone name: ") << getName() << " parent: " << tempNode->mName.data;
//////			mAiMatrixGlobal = tempNode->mTransformation * mAiMatrixGlobal;
////			mAiMatrixGlobal = tempNode->mTransformation * mAiMatrixGlobal;
////														 // boneMatrices[a] = boneMatrices[a] * tempNode->mTransformation;
//////			mAiMatrix = tempNode->mTransformation * mAiMatrix;
//////			mAiMatrix = mAiMatrix * tempNode->mTransformation;
////			tempNode = tempNode->mParent;
////		}
////		mAiMatrix = aGlobalInv * mAiMatrix;
////	} else {
////		mAiMatrix = mAiNode->mTransformation * mAiMatrix;
////		mAiMatrix = mAiMatrix * mAiNode->mTransformation;
////	}
//	if( bRoot) {
////		mAiMatrix = mAiMatrixGlobal;
//	}
//
////	mAiMatrix *= (mOffsetMatrix);
//
////	aiVector3t<float> tAiScale;
////	aiQuaterniont<float> tAiRotation;
////	aiVector3t<float> tAiPosition;
////
////	// this is the local matrix
////	mAiMatrix.Decompose( tAiScale, tAiRotation, tAiPosition );
////
////	glm::vec3 tpos = glm::vec3( tAiPosition.x, tAiPosition.y, tAiPosition.z );
////	glm::quat tquat = glm::quat(tAiRotation.w, tAiRotation.x, tAiRotation.y, tAiRotation.z);
//////	glm::quat tquat = glm::quat(tAiRotation.x, tAiRotation.y, tAiRotation.z, tAiRotation.w);
////	glm::vec3 tscale = glm::vec3( tAiScale.x, tAiScale.y, tAiScale.z );
////
//////	if( bRoot ) {
//////		std::cout << "Bone: " << getName() << " tscale: " << tscale << " | " << ofGetFrameNum() << std::endl;
//////	}
////
//////	if(bRoot) {
//////		setGlobalPosition( tpos );
//////		setGlobalOrientation( tquat );
//////		setScale( tscale );
//////	} else {
////		setPositionOrientationScale( tpos, tquat, tscale );
////	setPosition(tpos);
////	setOrientation(tquat);
////	setScale(tscale);
//
////	setGlobalPosition(tpos);
////	setGlobalOrientation(tquat);
////	setScale(tscale);
//
////	mPos = getGlobalPosition();
////	}
//
////	for(auto it = childBones.begin(); it != childBones.end(); ++it ) {
////		it->second->update();
////	}
////	for( auto& kid : childBones ) {
////		kid->update();
////	}
//
////	getGlobalTransformMatrix();
//}

//--------------------------------------------------------------
std::shared_ptr<ofx::assimp::SrcBone> SrcBone::getBone( aiNode* aAiNode ) {
	std::shared_ptr<ofx::assimp::SrcBone> tbone;
	findBoneRecursive( aAiNode, tbone );
	if( tbone ) {
		return tbone;
	}
	return tbone;
}

//--------------------------------------------------------------
void SrcBone::findBoneRecursive( aiNode* aAiNode, std::shared_ptr<SrcBone>& returnBone ) {
	if( !returnBone ) {
//		for(auto it = childBones.begin(); it != childBones.end(); ++it ) {
//			if( it->second->getAiNode() == aAiNode ) {
//				returnBone = (it->second);
//				break;
//			}
//			it->second->findBoneRecursive( aAiNode, returnBone );
//		}
		for(auto& kid : childBones ) {
			if( kid->getAiNode() == aAiNode ) {
				returnBone = kid;//(it->second);
				break;
			}
			kid->findBoneRecursive( aAiNode, returnBone );
		}
	}
}

//--------------------------------------------------------------
unsigned int SrcBone::getNumBones() {
	unsigned int ttotal = childBones.size();
//	for(auto it = childBones.begin(); it != childBones.end(); ++it ) {
//		ttotal += it->second->getNumBones();
//	}
	for(auto& kid : childBones ) {
		ttotal += kid->getNumBones();
	}
	return ttotal;
}

//--------------------------------------------------------------
std::string SrcBone::getAsString( unsigned int aLevel ) {
	std::stringstream oStr;// = "";
	for( unsigned int i = 0; i < aLevel; i++ ) {
		oStr << "  ";
		if( i < aLevel-1 ) {
			oStr << "|";
		}
	}
	if( aLevel > 0 ) {
//		oStr <<" '";
	}
	if( childBones.size() ) {
		oStr << "+ ";
	} else {
		oStr << "- ";
	}
//	std::string pname = "";
//	if( getParent() != NULL ) {
//		pname = " parent: " + parentBoneName;
//	}
	
	oStr << "Bone" << ": " << getName();// << pname << " fbx type: " << getFbxTypeString() << " anim: " << isAnimationEnabled();
	if( childBones.size() > 0 ) {
		oStr << " child bones: " << childBones.size();
	}
//	if(usingKeyFrames()) {
//		oStr << " num keys: " << mKeyCollections.size();
//	}
	oStr << std::endl;
	
	//    oStr << getTypeAsString()+": " + getName() + " kids: " +ofToString( childBones.size(), 0) + pname + " anim: " + ofToString( isAnimationEnabled(), 0) + " num keys: " +ofToString(mKeyCollections.size(),0) + "\n";
	
//	std::unordered_map<std::string, std::shared_ptr<SrcBone> >::iterator it;
//	for(auto it = childBones.begin(); it != childBones.end(); ++it ) {
//		oStr << it->second->getAsString( aLevel + 1);
//	}
	for(auto& kid : childBones ) {
		oStr << kid->getAsString( aLevel + 1);
	}
	return oStr.str();
}

