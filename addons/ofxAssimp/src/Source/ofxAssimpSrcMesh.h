//
//  ofxAssimpSrcMesh.h
//  ofxAssimpExample
//
//  Created by Nick Hardeman on 10/24/23.
//

#pragma once
#include "ofxAssimpSrcNode.h"
#include "ofxAssimpTexture.h"
#include "ofMaterial.h"
#include "ofVbo.h"
#include "ofxAssimpBounds.h"

namespace ofx::assimp {
class SrcMesh : public ofx::assimp::SrcNode {
public:
	static ofTexture sDummyTex;
	virtual NodeType getType() override { return OFX_ASSIMP_MESH; }
	
	void addTexture(std::shared_ptr<ofx::assimp::Texture> aAssimpTex);
	bool hasTexture();
	bool hasTexture(aiTextureType aTexType);
	bool hasTexture( ofMaterialTextureType aType );
	
	std::size_t getNumTextures();
	ofTexture& getTexture();
	ofTexture& getTexture(aiTextureType aTexType);
	ofTexture& getTexture(ofMaterialTextureType aType);
	std::vector<std::shared_ptr<ofx::assimp::Texture>> & getAllMeshTextures(){ return meshTextures; }
	
	void setAiMesh(aiMesh* amesh, aiNode* aAiNode);
	aiMesh* getAiMesh() { return mAiMesh; }
	const aiMesh* getAiMesh() const { return mAiMesh; }
	void setupVbo( std::shared_ptr<ofVbo> avbo );
	
	void setMeshFromAiMesh( ofMesh& amesh );
	void calculateLocalBounds(ofMesh& amesh);
	Bounds getLocalBounds() { return mLocalBounds; }
	
	std::shared_ptr<ofMaterial> material;
	std::shared_ptr<ofVbo> vbo;
	std::vector<ofIndexType> indices;
	
	ofBlendMode blendMode = OF_BLENDMODE_ALPHA;
	bool twoSided = false;
	bool bConvertedToLeftHand = true;
	bool bHasVboChanged = true;
	int usage = GL_STATIC_DRAW;
	
protected:
	aiMesh* mAiMesh = nullptr; // pointer to the aiMesh we represent.
	Bounds mLocalBounds;
	std::vector<std::shared_ptr<ofx::assimp::Texture>> meshTextures;
};
}
