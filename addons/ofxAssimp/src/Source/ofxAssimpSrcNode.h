//
//  ofxAssimpSrcNode.h
//  Created by Nick Hardeman on 10/19/23.
//

#pragma once
#include "ofxAssimpSrcAnimKeyCollection.h"
#include <assimp/scene.h>
#include <unordered_map>

namespace ofxAssimp {
enum NodeType {
	OFX_ASSIMP_MESH=0,
	OFX_ASSIMP_BONE,
	OFX_ASSIMP_SKELETON,
	OFX_ASSIMP_NODE,
	OFX_ASSIMP_SCENE
};

class SrcNode {
public:
	
	static std::string sGetNodeTypeAsString( const NodeType& atype );
	static std::string sGetNodeTypeShortAsString( const NodeType& atype );
	
	virtual NodeType getType() { return OFX_ASSIMP_NODE; }
	
	void setAiNode( aiNode* aAiNode );
	virtual std::string getName();
	std::string getTypeAsString() { return sGetNodeTypeAsString(getType()); }
	
	aiNode* getAiNode() { return mAiNode; }
	
	std::shared_ptr<ofxAssimp::SrcNode> getNode( aiNode* aAiNode );
	void findNodeRecursive( aiNode* aAiNode, std::shared_ptr<SrcNode>& aReturnNode );
	
	std::shared_ptr<ofxAssimp::SrcNode> getNode( const std::string& aAiNodeName );
	void findNodeRecursive( const std::string& aAiNodeName, std::shared_ptr<SrcNode>& aReturnNode );
	
	void clearChildren();
	void addChild( std::shared_ptr<ofxAssimp::SrcNode> akiddo );
	unsigned int getNumChildren();
	std::vector< std::shared_ptr<ofxAssimp::SrcNode> >& getChildren();
	
	virtual std::string getAsString( int aLevel=0 );
	
	// animation functions
	ofxAssimp::SrcAnimKeyCollection& getKeyCollection( unsigned int aAnimUId );
	
protected:
	aiNode* mAiNode = nullptr;
	std::string mName = "";
	std::vector< std::shared_ptr<ofxAssimp::SrcNode> > mKids;
	
	std::unordered_map<unsigned int, ofxAssimp::SrcAnimKeyCollection> mKeyCollections;
};
}
