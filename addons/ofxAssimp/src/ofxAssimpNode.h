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
	void setAnimationMixer( std::shared_ptr<AnimationMixer> amixer );
	void removeAnimationMixer();
	std::shared_ptr<ofx::assimp::AnimationMixer> getAnimationMixer() {return mAnimMixer;}
	
	void setParentNode( std::shared_ptr<ofx::assimp::Node> anode );
	bool hasParentNode();
	std::shared_ptr<ofx::assimp::Node> getParentNode();
	
	void clearChildren();
	void addChild( std::shared_ptr<ofx::assimp::Node> akiddo );
	int getNumChildren();
	std::vector< std::shared_ptr<ofx::assimp::Node> >& getChildren();
	
	virtual std::string getAsString( int aLevel=0 );
	
	glm::mat4& getGlobalTransformCached();
	
	glm::vec3& getGlobalPositionCached();
	glm::quat& getGlobalOrientationCached();
	glm::vec3& getGlobalScaleCached();
	
	std::shared_ptr<ofx::assimp::Node> getNode( const std::string& aPath, bool bStrict );
	
	// some getter functions to help with the search on the kids
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
	
	template<typename ofxAssimpNodeType>
	std::vector< std::shared_ptr<ofxAssimpNodeType> > getNodesForType( const std::string& aNameToContain="" ) {
		int sType = std::make_shared<ofxAssimpNodeType>()->getType();
		auto stype = typeid(ofxAssimpNodeType).name();
		
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
	
	glm::vec3 mCachedGPos = {0.0f,0.0f,0.0f};
	glm::quat mCachedGQuat;
	glm::vec3 mCachedGScale = {1.0f,1.0f,1.0f};
	glm::mat4 mCachedGlobalTransform = glm::mat4(1.0f);
	
	bool bCachedGTransformDirty = true;
	bool bCachedGPosDirty = true;
	bool bCachedGOrientationDirty = true;
	bool bCachedGScaleDirty = true;
	
	
	// ofNode functions called when it changes
	virtual void onPositionChanged() override;
	
	/// \brief Classes extending ofNode can override this methods to get notified
	///        when the orientation changed.
	virtual void onOrientationChanged() override;
	
	/// \brief Classes extending ofNode can override this methods to get notified
	///        when the scale changed.
	virtual void onScaleChanged() override;
};
}
