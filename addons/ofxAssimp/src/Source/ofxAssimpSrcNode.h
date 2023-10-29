//
//  ofxAssimpSrcNode.h
//  Created by Nick Hardeman on 10/19/23.
//

#pragma once
#include "ofNode.h"
#include <assimp/scene.h>

namespace ofx::assimp {
enum NodeType {
	OFX_ASSIMP_MESH=0,
	OFX_ASSIMP_BONE,
	OFX_ASSIMP_SKELETON,
	OFX_ASSIMP_NODE
};

class SrcNode {
public:
	
	static std::string sGetNodeTypeAsString( const NodeType& atype );
	
	virtual NodeType getType() { return OFX_ASSIMP_NODE; }
	
	void setAiNode( aiNode* aAiNode );
	virtual std::string getName();
	std::string getTypeAsString() { return sGetNodeTypeAsString(getType()); }
	
	aiNode* getAiNode() { return mAiNode; }
	
	std::shared_ptr<ofx::assimp::SrcNode> getNode( aiNode* aAiNode );
	void findNodeRecursive( aiNode* aAiNode, std::shared_ptr<SrcNode>& returnBone );
	
	void clearChildren();
	void addChild( std::shared_ptr<ofx::assimp::SrcNode> akiddo );
	unsigned int getNumChildren();
	std::vector< std::shared_ptr<ofx::assimp::SrcNode> >& getChildren();
	
	virtual std::string getAsString( int aLevel=0 );
	
protected:
	aiNode* mAiNode = nullptr;
	std::string mName = "";
	std::vector< std::shared_ptr<ofx::assimp::SrcNode> > mKids;
};
}
