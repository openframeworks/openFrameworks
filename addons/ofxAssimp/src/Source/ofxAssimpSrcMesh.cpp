//
//  ofxAssimpSrcMesh.cpp
//  ofxAssimpExample
//
//  Created by Nick Hardeman on 10/24/23.
//

#include "ofxAssimpSrcMesh.h"
#include "ofxAssimpUtils.h"
#include "of3dGraphics.h"
#include "ofGraphics.h"

using std::make_shared;
using std::shared_ptr;

using namespace ofx::assimp;

static unsigned int sUniqueMeshCounter = 0;
ofTexture SrcMesh::sDummyTex;

//-------------------------------------------
void SrcMesh::addTexture( std::shared_ptr<ofx::assimp::Texture> aAssimpTex){
	
	if(!material) {
		material = std::make_shared<ofMaterial>();
	}
	
	auto tAiType = aAssimpTex->getAiTextureType();
	auto tofType = ofx::assimp::Texture::ofTextureTypeForAiType(tAiType);
	
	if(tofType == OF_MATERIAL_TEXTURE_NONE ) {
		ofLogError("ofx::assimp::Mesh::addTexture") << aAssimpTex->getAiTextureTypeAsString();
		return;
	}
	
	bool bGoAheadAndAdd = false;
	
	if( tofType == OF_MATERIAL_TEXTURE_DIFFUSE) {
		if(!hasTexture(OF_MATERIAL_TEXTURE_DIFFUSE)) {
			bGoAheadAndAdd = true;
		}
	} else {
		if( tofType != OF_MATERIAL_TEXTURE_NONE ) {
			bGoAheadAndAdd = true;
		}
	}
	
	if(bGoAheadAndAdd) {
//		auto nAssimpTex = std::make_shared<ofxAssimpTexture>();
//		(*nAssimpTex.get()) = aAssimpTex;
		meshTextures.push_back(aAssimpTex);
		material->setTexture(tofType, aAssimpTex->getTextureRef());
	}
}

//-------------------------------------------
bool SrcMesh::hasTexture() {
	return meshTextures.size() > 0;
}

//-------------------------------------------
bool SrcMesh::hasTexture(aiTextureType aTexType){
	for( auto& tex : meshTextures ){
		if( tex && tex->getAiTextureType() == aTexType){
			return tex->hasTexture();
		}
	}
	return false;
}

//-------------------------------------------
bool SrcMesh::hasTexture(ofMaterialTextureType aType){
	return hasTexture( ofx::assimp::Texture::aiTextureTypeForOfType(aType));
}

//-------------------------------------------
std::size_t SrcMesh::getNumTextures() {
	return meshTextures.size();
}

//-------------------------------------------
ofTexture& SrcMesh::getTexture() {
	for( auto iter = ofx::assimp::Texture::sAiTexTypeToOfTexTypeMap.begin(); iter != ofx::assimp::Texture::sAiTexTypeToOfTexTypeMap.end(); iter++ ) {
		if( hasTexture((aiTextureType)iter->first) ) {
			return getTexture((aiTextureType)iter->first);
		}
	}
	
	ofLogWarning("ofx::assimp::Mesh::getTexture") << " unable to find any allocated texture";
	return sDummyTex;
}

//-------------------------------------------
ofTexture& SrcMesh::getTexture(aiTextureType aTexType){
	for( auto & tex : meshTextures ){
		if( tex && tex->getAiTextureType() == aTexType){
			return tex->getTextureRef();
		}
	}
	ofLogWarning("ofx::assimp::SrcMesh::getTexture : unable to find texture ref for ") << aTexType;
	return sDummyTex;
}

//-------------------------------------------
ofTexture& SrcMesh::getTexture(ofMaterialTextureType aType){
	return getTexture( ofx::assimp::Texture::aiTextureTypeForOfType(aType) );
}

//-------------------------------------------
void SrcMesh::setAiMesh( aiMesh* amesh, aiNode* aAiNode ) {
	SrcNode::setAiNode(aAiNode);
	mAiMesh = amesh;
	if( mAiMesh != NULL ) {
		vbo = std::make_shared<ofVbo>();
		mName = mAiMesh->mName.data;
		getName(); // forces setting of the name
//		if(!material) {
//			material = std::make_shared<ofMaterial>();
//		}
		mName = mAiMesh->mName.data;
		if(mName.empty() ) {
			// the mesh is not named or we were unable to detect one
			mName = "Default Mesh "+ofToString(sUniqueMeshCounter,0);
			if( sUniqueMeshCounter > std::numeric_limits<unsigned int>().max()-3) {
				sUniqueMeshCounter = 0;
			}
		}
	}
}

//-------------------------------------------
void SrcMesh::setupVbo( std::shared_ptr<ofVbo> avbo ) {
	ofMesh tempMesh;
	if( hasTexture() ) {
		aiMeshToOfMesh(mAiMesh, tempMesh, !bConvertedToLeftHand, &getTexture() );
	} else {
		aiMeshToOfMesh(mAiMesh, tempMesh, !bConvertedToLeftHand, nullptr);
	}
		
	avbo->setVertexData(&mAiMesh->mVertices[0].x,3,mAiMesh->mNumVertices,usage,sizeof(aiVector3D));
	if(mAiMesh->HasVertexColors(0)){
		avbo->setColorData(&mAiMesh->mColors[0][0].r,mAiMesh->mNumVertices,GL_STATIC_DRAW,sizeof(aiColor4D));
	}
	if(mAiMesh->HasNormals()){
		avbo->setNormalData(&mAiMesh->mNormals[0].x,mAiMesh->mNumVertices,usage,sizeof(aiVector3D));
	}
	if (tempMesh.hasTexCoords()){
		avbo->setTexCoordData(&tempMesh.getTexCoords()[0].x, mAiMesh->mNumVertices,GL_STATIC_DRAW,sizeof(glm::vec2));
	}
	
	std::vector<ofIndexType> tempIndices;
	tempIndices.resize(mAiMesh->mNumFaces * 3);
	int j=0;
	for (unsigned int x = 0; x < mAiMesh->mNumFaces; ++x){
		for (unsigned int a = 0; a < mAiMesh->mFaces[x].mNumIndices; ++a){
			tempIndices[j++] = mAiMesh->mFaces[x].mIndices[a];
		}
	}
	
	avbo->setIndexData(&tempIndices[0],tempIndices.size(),GL_STATIC_DRAW);
}

//-------------------------------------------
void SrcMesh::setMeshFromAiMesh( ofMesh& amesh ) {
	if( mAiMesh != NULL && amesh.getNumVertices() < 1 ) {
		if( hasTexture() ) {
			aiMeshToOfMesh(mAiMesh, amesh, !bConvertedToLeftHand, &getTexture());
		} else {
			aiMeshToOfMesh(mAiMesh, amesh, !bConvertedToLeftHand, nullptr);
		}
	}
}

//-------------------------------------------
void SrcMesh::calculateLocalBounds(ofMesh& amesh) {
	mLocalBounds.clear();
	mLocalBounds.include(amesh.getVertices());
}


