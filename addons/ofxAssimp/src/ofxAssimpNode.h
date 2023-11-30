//
//  ofxAssimpNode.h
//  Created by Nick Hardeman on 10/24/23.
//

#pragma once
#include "ofNode.h"
#include "ofxAssimpAnimationMixer.h"
#include "ofxAssimpSrcNode.h"
#include <assimp/scene.h>

namespace ofx::assimp {
class Node : public ofNode {
public:
	void setSrcNode( std::shared_ptr<ofx::assimp::SrcNode> aSrcNode );
	virtual std::string getName();
	
	virtual NodeType getType() { return OFX_ASSIMP_NODE; }
	
	virtual void update() {};
	virtual void update( const std::shared_ptr<ofx::assimp::AnimationMixer>& aAnimMixer );
	
	virtual void drawNodes();
	
	bool hasAnimationMixer();
	/// \brief Set animation mixer on node propogates down children (recursive).
	/// \param amixer AnimationMixer to set on the node.
	void setAnimationMixer( std::shared_ptr<AnimationMixer> amixer );
	/// \brief Remove the animation mixer from this node.
	void removeAnimationMixer();
	/// \brief Get the animation mixer on the node.
	/// \return AnimationMixer as a shared ptr.
	std::shared_ptr<ofx::assimp::AnimationMixer> getAnimationMixer() {return mAnimMixer;}
	
	bool areAnimationsEnabled() const;
	void setAnimationsEnabled(bool aBEnable, bool aBRecursively=true);
	
	bool isEnabled() const;
	void setEnabled( bool aBEnable, bool aBRecursively=true);
	
	void setParentNode( std::shared_ptr<ofx::assimp::Node> anode );
	bool hasParentNode();
	std::shared_ptr<ofx::assimp::Node> getParentNode();
	
	void clearChildren();
	void addChild( std::shared_ptr<ofx::assimp::Node> akiddo );
	int getNumChildren();
	std::vector< std::shared_ptr<ofx::assimp::Node> >& getChildren();
	
	virtual std::string getAsString( int aLevel=0 );
	/// \brief Get a node for path.
	/// \param aPath path in heirarchy to the desired node.\nUse direct path, ie. 'root:someNode:myNode' or recursively using asterik ie. '*:myNode'.
	/// \param bStrict if true, name must match. If not, apath search name is contained in node string.
	/// \return ofx::assimp::Node as shared_ptr. Valid ptr if found.
	std::shared_ptr<ofx::assimp::Node> getNode( const std::string& aPath, bool bStrict );
		
	/// \brief Get a node cast as template type. ie. getNodeAsType<ofx::assimp::Bone>('root:someNode:myNode', true);
	/// \param aPath path in heirarchy to the desired node.\nUse direct path, ie. 'root:someNode:myNode' or recursively using asterik ie. '*:myNode'.
	/// \param bStrict if true, name must match. If not, apath search name is contained in node string.
	/// \return ofx::assimp::Node cast to ofxAssimpNodeType as shared_ptr. Valid ptr if found.
	template<typename ofxAssimpNodeType>
	std::shared_ptr<ofxAssimpNodeType> getNodeAsType( const std::string& aPath, bool bStrict = false ) {
		auto stemp = getNode( aPath, bStrict );
		if( stemp ) {
			int sType = std::make_shared<ofxAssimpNodeType>()->getType();
			if(stemp->getType() == sType) {
				return std::dynamic_pointer_cast<ofxAssimpNodeType>( stemp );
			} else {
				ofLogWarning("ofx::assimp::Node") << getName() << " found node with path: " << aPath << ", but WRONG type!";
			}
		}
		return std::shared_ptr<ofxAssimpNodeType>();
	}
	/// \brief Get child nodes for template type ie. getNodesForType<ofx::assimp::Bone>(); (non-recursive).
	/// \param aNameToContain To be returned, name of node must contain this string.\nDefault is return all nodes for type.
	/// \return vector of shared_ptrs of ofx::assimp::Node cast to ofxAssimpNodeType.
	template<typename ofxAssimpNodeType>
	std::vector< std::shared_ptr<ofxAssimpNodeType> > getNodesForType( const std::string& aNameToContain="" ) {
		int sType = std::make_shared<ofxAssimpNodeType>()->getType();
		std::vector< std::shared_ptr<ofxAssimpNodeType> > telements;
		for( size_t i = 0; i < mKids.size(); i++ ) {
			if( mKids[i]->getType() == sType ) {
				if( aNameToContain == "" || ofIsStringInString( mKids[i]->getName(), aNameToContain )) {
					telements.push_back(std::dynamic_pointer_cast<ofxAssimpNodeType>(mKids[i]) );
				}
			}
		}
		return telements;
	}
	/// \brief Get nodes for template type ie. getNodesForType<ofx::assimp::Bone>(); (recursive).
	/// \param aNameToContain To be returned, name of node must contain this string.\nDefault is return all nodes for type.
	/// \return vector of shared_ptrs of ofx::assimp::Node cast to ofxAssimpNodeType.
	template<typename ofxAssimpNodeType>
	std::vector< std::shared_ptr<ofxAssimpNodeType> > getAllNodesForType( const std::string& aNameToContain="" ) {
		int sType = std::make_shared<ofxAssimpNodeType>()->getType();
		std::vector< std::shared_ptr<ofx::assimp::Node> > tFoundNodes;
		_getNodesForTypeRecursive( sType, aNameToContain, tFoundNodes, mKids );
		std::vector< std::shared_ptr<ofxAssimpNodeType> > tReturnNodes;
		for( auto tfn : tFoundNodes ) {
			tReturnNodes.push_back( std::dynamic_pointer_cast<ofxAssimpNodeType>(tfn) );
		}
		return tReturnNodes;
	}
	
protected:
	void _getNodeForNameRecursive( std::vector<std::string>& aNamesToFind,
								   std::shared_ptr<ofx::assimp::Node>& aTarget,
								  std::vector< std::shared_ptr<ofx::assimp::Node> >& aElements, bool bStrict );
	void _getNodesForTypeRecursive( int atype, const std::string& aNameToContain,
								  std::vector< std::shared_ptr<ofx::assimp::Node> >& aFoundElements,
								  std::vector< std::shared_ptr<ofx::assimp::Node> >& aElements );
	
	std::string mName = "";
	std::shared_ptr<ofx::assimp::SrcNode> mSrcNode;
	std::vector< std::shared_ptr<ofx::assimp::Node> > mKids;
	std::shared_ptr<ofx::assimp::Node> mParentNode;
	
	std::shared_ptr<ofx::assimp::AnimationMixer> mAnimMixer;
	
	bool mBAnimationsEnabled = true;
	bool mBEnabled = true;
	
};
}
