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
	if( hasAnimationMixer() && areAnimationsEnabled() ) {
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
	if( mSrcNode && areAnimationsEnabled() ) {
		
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
bool Node::areAnimationsEnabled() const {
	return mBAnimationsEnabled && isEnabled();
}

//----------------------------------------
void Node::setAnimationsEnabled(bool aBEnable,bool aBRecursively) {
	mBAnimationsEnabled = aBEnable;
	if(aBRecursively) {
		// loop through children //
		for( auto& kid : mKids ) {
			kid->setAnimationsEnabled(aBEnable);
		}
	}
}

//----------------------------------------
bool Node::isEnabled() const {
	return mBEnabled;
}

//----------------------------------------
void Node::setEnabled( bool aBEnable, bool aBRecursively) {
	mBEnabled = aBEnable;
	if(aBRecursively) {
		// loop through children //
		for( auto& kid : mKids ) {
			kid->setEnabled(aBEnable);
		}
	}
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
	std::stringstream oStr;
	for( unsigned int i = 0; i < aLevel; i++ ) {
		oStr << " ";
		if( i < aLevel-1 ) {
			oStr << "|";
		}
	}
	if( mKids.size() ) {
		oStr << "+ ";
	} else {
		oStr << "- ";
	}
	oStr << ofx::assimp::SrcNode::sGetNodeTypeShortAsString( getType() );
	oStr << ": " << getName();
	oStr << std::endl;
	
	for( auto& kid : mKids ) {
		oStr << kid->getAsString( aLevel + 1);
	}
	
	return oStr.str();
}
