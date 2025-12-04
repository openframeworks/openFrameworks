#include "ofxAssimpSrcBone.h"
#include "ofGraphics.h"
#include "of3dGraphics.h"
#include "of3dUtils.h"
#include "ofxAssimpUtils.h"

using namespace ofxAssimp;

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

//--------------------------------------------------------------
std::shared_ptr<ofxAssimp::SrcBone> SrcBone::getBone( aiNode* aAiNode ) {
	std::shared_ptr<ofxAssimp::SrcBone> tbone;
	findBoneRecursive( aAiNode, tbone );
	if( tbone ) {
		return tbone;
	}
	return tbone;
}

//--------------------------------------------------------------
void SrcBone::findBoneRecursive( aiNode* aAiNode, std::shared_ptr<SrcBone>& returnBone ) {
	if( !returnBone ) {
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
	unsigned int ttotal = (unsigned int)childBones.size();
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

