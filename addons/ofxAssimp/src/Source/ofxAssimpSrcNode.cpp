#include "ofxAssimpSrcNode.h"

static unsigned int sUniqueIdCounter = 0;

using namespace ofx::assimp;

//--------------------------------------------------------------
std::string SrcNode::sGetNodeTypeAsString( const NodeType& atype ) {
	if( atype == OFX_ASSIMP_MESH ) {
		return "OFX_ASSIMP_MESH";
	} else if(atype == OFX_ASSIMP_BONE ) {
		return "OFX_ASSIMP_BONE";
	} else if(atype == OFX_ASSIMP_SKELETON ) {
		return "OFX_ASSIMP_SKELETON";
	} else if(atype == OFX_ASSIMP_MODEL ) {
		return "OFX_ASSIMP_MODEL";
	}
	return "OFX_ASSIMP_NODE";
}

//--------------------------------------------------------------
std::string SrcNode::sGetNodeTypeShortAsString( const NodeType& atype ) {
	if( atype == OFX_ASSIMP_MESH ) {
		return "MESH";
	} else if(atype == OFX_ASSIMP_BONE ) {
		return "BONE";
	} else if(atype == OFX_ASSIMP_SKELETON ) {
		return "SKELETON";
	} else if(atype == OFX_ASSIMP_MODEL ) {
		return "MODEL";
	}
	return "NODE";
}

//--------------------------------------------------------------
void SrcNode::setAiNode( aiNode* aAiNode ) {
	mAiNode = aAiNode;
}

//-------------------------------------------
std::string SrcNode::getName() {
	if( mName.empty() && mAiNode != NULL ) {
		mName = mAiNode->mName.data;
	}
	if(mName.empty() ) {
		// the mesh is not named or we were unable to detect one
		mName = "Default "+ofToString(sUniqueIdCounter,0);
		if( sUniqueIdCounter > std::numeric_limits<unsigned int>().max()-3) {
			sUniqueIdCounter = 0;
		}
	}
	return mName;
}

//----------------------------------------
std::shared_ptr<ofx::assimp::SrcNode> SrcNode::getNode( aiNode* aAiNode ) {
	std::shared_ptr<ofx::assimp::SrcNode> tnode;
	findNodeRecursive( aAiNode, tnode );
	return tnode;
}

//----------------------------------------
void SrcNode::findNodeRecursive( aiNode* aAiNode, std::shared_ptr<SrcNode>& aReturnNode ) {
	if( !aReturnNode ) {
		for(auto& kid : mKids ) {
			if( kid->getAiNode() == aAiNode ) {
				aReturnNode = kid;
				break;
			}
			kid->findNodeRecursive( aAiNode, aReturnNode );
		}
	}
}

//----------------------------------------
std::shared_ptr<ofx::assimp::SrcNode> SrcNode::getNode( const std::string& aAiNodeName ) {
	std::shared_ptr<ofx::assimp::SrcNode> tnode;
	findNodeRecursive( aAiNodeName, tnode );
	return tnode;
}

//----------------------------------------
void SrcNode::findNodeRecursive( const std::string& aAiNodeName, std::shared_ptr<SrcNode>& aReturnNode ) {
	if( !aReturnNode ) {
		for(auto& kid : mKids ) {
			if( kid->getName() == aAiNodeName ) {
				aReturnNode = kid;
				break;
			}
			kid->findNodeRecursive( aAiNodeName, aReturnNode );
		}
	}
}

//----------------------------------------
void SrcNode::clearChildren() {
	mKids.clear();
}

//----------------------------------------
void SrcNode::addChild( std::shared_ptr<SrcNode> akiddo ) {
	mKids.push_back( akiddo );
}

//----------------------------------------
unsigned int SrcNode::getNumChildren() {
	return mKids.size();
}

//----------------------------------------
std::vector< std::shared_ptr<SrcNode> >& SrcNode::getChildren() {
	return mKids;
}

//--------------------------------------------------------------
std::string SrcNode::getAsString( int aLevel ) {
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
	if( mKids.size() ) {
		oStr << "+ ";
	} else {
		oStr << "- ";
	}
//    string pname = "";
//    if( getParent() != NULL ) {
//        pname = " parent: " + parentBoneName;
//    }
	oStr << getTypeAsString() << ": " << getName();
	if( getNumChildren() > 0 ) {
		oStr << " kids: " << mKids.size();
	}
//	if(usingKeyFrames()) {
//		oStr << " num keys collections: " << mKeyCollections.size();
//	}
	oStr << std::endl;// "\n";
	
	for( auto& kid : mKids ) {
		oStr << kid->getAsString( aLevel + 1);
	}
	
	return oStr.str();
}

// animation functions
//--------------------------------------------------------------
ofx::assimp::SrcAnimKeyCollection& SrcNode::getKeyCollection(unsigned int aAnimUId){
	if( mKeyCollections.count(aAnimUId) < 1 ) {
		SrcAnimKeyCollection temp;
		temp.uId = aAnimUId;
		mKeyCollections[ aAnimUId ] = temp;
	}
	return mKeyCollections[aAnimUId];
}


