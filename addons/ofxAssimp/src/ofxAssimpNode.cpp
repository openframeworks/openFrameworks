//
//  ofxAssimpNode.cpp
//  Created by Nick Hardeman on 10/24/23.
//

#include "ofxAssimpNode.h"
#include "ofxAssimpUtils.h"
#include "of3dGraphics.h"

using namespace ofx::assimp;

using std::shared_ptr;
using std::string;
using std::cout;

//--------------------------------------------------------------
void Node::setSrcNode( std::shared_ptr<ofx::assimp::SrcNode> aSrcNode ) {
	mSrcNode = aSrcNode;
	if( getType() == OFX_ASSIMP_BONE ) {
//		setOfNodeFromAiMatrix(mSrcNode->getAiNode()->mTransformation, this );
	}
}

//--------------------------------------------------------------
std::string Node::getName() {
	if(mName.empty() && mSrcNode) {
		return mSrcNode->getName();
	}
	return "Default";
}

//--------------------------------------------------------------
void Node::update( const std::shared_ptr<ofx::assimp::AnimationMixer>& aAnimMixer ) {
	std::shared_ptr<ofx::assimp::AnimationMixer> mixer;
	if( hasAnimationMixer() ) {
		mixer = mAnimMixer;
	} else {
		mixer = aAnimMixer;
	}

	if( !mixer ) {
		ofLogError("ofx::assimp::Node:update") << "not a valid AnimationMixer!";
		return;
	}
	
	// we have an animation mixer, override the incoming mixer //
	// first get the appropriate key collection
	if( mSrcNode ) {
		
		auto numClips = mixer->getNumAnimationClips();
		if( numClips > 0 ) {
			glm::vec3 tempPos = {0.f, 0.f, 0.f};
			glm::vec3 tempScale = {0.f, 0.f, 0.f};
			glm::quat tempQuat = glm::quat(1.f, 0.f, 0.f, 0.f);
			bool bHasSomeKeys = false;
			int clipWithKeysIndex = 0;
			for( auto& animClip : mixer->getAnimationClips() ) {
				auto& keyCollection = mSrcNode->getKeyCollection(animClip.animation.getUid());
				
				if( keyCollection.hasKeys() ) {
					auto animTime = animClip.animation.getPositionInTicks() + animClip.animation.getStartTick();
					tempPos += animClip.weight * keyCollection.getPosition( animTime );
					tempScale += animClip.weight * keyCollection.getScale( animTime );
					// TODO: Keep an eye on this :O
					if(clipWithKeysIndex == 0 ) {
						tempQuat = keyCollection.getRotation(animTime);
					} else {
						tempQuat = glm::slerp(tempQuat, keyCollection.getRotation(animTime), animClip.weight );
					}
					
					clipWithKeysIndex++;
					bHasSomeKeys = true;
				}
			}
			if( bHasSomeKeys ) {
//				if( getName() == "astroBoy_newSkeleton_R_middle_01") {
//					ofLogNotice("ofx::assimp::Node::update") << " has keys: " << bHasSomeKeys;
//				}
				setPositionOrientationScale( tempPos, tempQuat, tempScale );
			}
		}
	}
	
	for( auto& kid : mKids ) {
		kid->update(mixer);
	}
}

//----------------------------------------
void Node::drawNodes() {
	ofDrawSphere( getGlobalPosition(), 5.0);
	for( auto& kid : mKids ) {
		kid->drawNodes();
	}
}

//----------------------------------------
bool Node::hasAnimationMixer() {
	if(mAnimMixer) {
		return true;
	}
	return false;
}

//----------------------------------------
void Node::setAnimationMixer( std::shared_ptr<AnimationMixer> amixer ) {
	mAnimMixer = amixer;
}

//----------------------------------------
void Node::removeAnimationMixer() {
	mAnimMixer.reset();
}

//----------------------------------------
void Node::setParentNode( shared_ptr<ofx::assimp::Node> anode ) {
	mParentNode = anode;
}

//----------------------------------------
bool Node::hasParentNode() {
	if( !mParentNode ) return false;
	return true;
}

//----------------------------------------
shared_ptr<ofx::assimp::Node> Node::getParentNode() {
	return mParentNode;
}

//----------------------------------------
void Node::clearChildren() {
	mKids.clear();
}

//----------------------------------------
void Node::addChild( std::shared_ptr<Node> akiddo ) {
	mKids.push_back( akiddo );
}

//----------------------------------------
int Node::getNumChildren() {
	return mKids.size();
}

//----------------------------------------
std::vector< std::shared_ptr<Node> >& Node::getChildren() {
	return mKids;
}

//--------------------------------------------------------------
std::shared_ptr<ofx::assimp::Node> Node::getNode( const string& aPath, bool bStrict ) {
	std::vector< std::string > tsearches;
	if( ofIsStringInString( aPath, ":" ) ) {
		tsearches = ofSplitString( aPath, ":" );
	} else {
		tsearches.push_back( aPath );
	}
	std::shared_ptr<ofx::assimp::Node> temp;
	_getNodeForNameRecursive( tsearches, temp, mKids, bStrict );
	return temp;
}

//--------------------------------------------------------------
void Node::_getNodeForNameRecursive(std::vector<std::string>& aNamesToFind,
									std::shared_ptr<ofx::assimp::Node>& aTarget,
									std::vector< std::shared_ptr<ofx::assimp::Node> >& aElements, bool bStrict ) {
	if( aNamesToFind.size() < 1 ) {
		return;
	}
	if(aTarget) {
		return;
	}
	bool bKeepGoing = false;
	std::string nameToFind = aNamesToFind[0];
	if( aNamesToFind.size() > 1 ) {
		bKeepGoing = (aNamesToFind[0] == "*");
		nameToFind = aNamesToFind[1];
	}
	for( size_t i = 0; i < aElements.size(); i++ ) {
		bool bFound = false;
		if(bStrict) {
			if( aElements[i]->getName() == nameToFind ) {
				bFound = true;
			}
		} else {
			if( ofIsStringInString( aElements[i]->getName(), nameToFind )) {
				bFound = true;
			}
		}
		
		if( bFound && !bKeepGoing ) {
			if( !bKeepGoing && aNamesToFind.size() > 0 ) {
				aNamesToFind.erase( aNamesToFind.begin() );
			}
			if(aNamesToFind.size() == 0 ) {
				aTarget = aElements[i];
				break;
			} else {
				if( aElements[i]->getNumChildren() > 0 ) {
					_getNodeForNameRecursive( aNamesToFind, aTarget, aElements[i]->getChildren(), bStrict );
					break;
				}
			}
		}
		
		if( bKeepGoing ) {
			if( bFound ) {
				aTarget = aElements[i];
				break;
			} else {
				if( aElements[i]->getNumChildren() > 0 ) {
					_getNodeForNameRecursive( aNamesToFind, aTarget, aElements[i]->getChildren(), bStrict );
				}
			}
		}
	}
}

//--------------------------------------------------------------
void Node::_getNodesForTypeRecursive( int atype, const std::string& aNameToContain,
									std::vector< std::shared_ptr<ofx::assimp::Node> >& aFoundElements,
									std::vector< std::shared_ptr<ofx::assimp::Node> >& aElements ) {
	
	for( size_t i = 0; i < aElements.size(); i++ ) {
		if( aElements[i]->getType() == atype ) {
			bool bFound = false;
			if( aNameToContain != "" ) {
				if( ofIsStringInString( aElements[i]->getName(), aNameToContain )) {
					bFound = true;
				}
			} else {
				bFound = true;
			}
			if( bFound ) {
				aFoundElements.push_back( aElements[i] );
			}
		}
		
		if( aElements[i]->getNumChildren() > 0 ) {
			_getNodesForTypeRecursive( atype, aNameToContain, aFoundElements, aElements[i]->getChildren() );
		}
	}
}

//--------------------------------------------------------------
std::string Node::getAsString( int aLevel ) {
	std::stringstream oStr;// = "";
	for( unsigned int i = 0; i < aLevel; i++ ) {
//		if( i == aLevel -1 ) {
//			oStr << "-";
//		} else {
			oStr << " ";
//		}
		if( i < aLevel-1 ) {
			oStr << "|";
		}
	}
	if( aLevel > 0 ) {
		//		oStr <<" '";
	}
	if( mKids.size() ) {
		oStr << "+ ";
	} else {
		oStr << "- ";
	}
	//    string pname = "";
	//    if( getParent() != NULL ) {
	//        pname = " parent: " + parentBoneName;
	//    }
//	if(mSrcNode) {
//		oStr << mSrcNode->getTypeAsString();
//	}
	oStr << ofx::assimp::SrcNode::sGetNodeTypeShortAsString( getType() );
	oStr << ": " << getName();
//	if( getNumChildren() > 0 ) {
//		oStr << " kids: " << mKids.size();
//	}
	//	if(usingKeyFrames()) {
	//		oStr << " num keys collections: " << mKeyCollections.size();
	//	}
	oStr << std::endl;// "\n";
	
	for( auto& kid : mKids ) {
		oStr << kid->getAsString( aLevel + 1);
	}
	
	return oStr.str();
}

////--------------------------------------------------------------
//glm::mat4& Node::getGlobalTransformCached() {
//	if(bCachedGTransformDirty) {
//		bCachedGTransformDirty = false;
//		mCachedGlobalTransform = getGlobalTransformMatrix();
//	}
//	return mCachedGlobalTransform;
//}
//
////--------------------------------------------------------------
//glm::vec3& Node::getGlobalPositionCached() {
//	if(bCachedGPosDirty) {
//		bCachedGPosDirty = false;
//		mCachedGPos = glm::vec3(getGlobalTransformCached()[3]);
//	}
//	return mCachedGPos;
//}
//
////--------------------------------------------------------------
//glm::quat& Node::getGlobalOrientationCached() {
//	if(bCachedGOrientationDirty) {
//		bCachedGOrientationDirty = false;
//		mCachedGQuat = getGlobalOrientation();
//	}
//	return mCachedGQuat;
//}
//
////--------------------------------------------------------------
//glm::vec3& Node::getGlobalScaleCached() {
//	if(bCachedGScaleDirty) {
//		bCachedGScaleDirty = false;
//		mCachedGScale = getGlobalScale();
//	}
//	return mCachedGScale;
//}

//--------------------------------------------------------------
void Node::onPositionChanged() {
	bCachedGTransformDirty=true;
	bCachedGPosDirty = true;
}

//--------------------------------------------------------------
void Node::onOrientationChanged() {
	bCachedGTransformDirty=true;
	bCachedGOrientationDirty = true;
}

//--------------------------------------------------------------
void Node::onScaleChanged() {
	bCachedGTransformDirty=true;
	bCachedGScaleDirty = true;
}
