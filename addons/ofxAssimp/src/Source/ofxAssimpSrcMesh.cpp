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
std::unordered_map< int, ofMaterialTextureType > SrcMesh::sAiTexTypeToOfTexTypeMap;

ofTexture SrcMesh::sDummyTex;

//-------------------------------------------
void SrcMesh::addTexture( std::shared_ptr<ofx::assimp::Texture> aAssimpTex){
	
	if(!material) {
		material = std::make_shared<ofMaterial>();
	}
	
	auto tAiType = aAssimpTex->getTextureType();
	auto tofType = _getOfTypeForAiType(tAiType);
	
	if(tofType == OF_MATERIAL_TEXTURE_NONE ) {
		ofLogError("ofx::assimp::Mesh::addTexture") << aAssimpTex->getTextureTypeAsString();
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
ofTexture& SrcMesh::getTexture() {
	_initTextureTypeMap();
	for( auto iter = sAiTexTypeToOfTexTypeMap.begin(); iter != sAiTexTypeToOfTexTypeMap.end(); iter++ ) {
		if( hasTexture((aiTextureType)iter->first) ) {
			return getTexture((aiTextureType)iter->first);
		}
	}
	ofLogWarning("ofx::assimp::Mesh::getTexture") << " unable to find any allocated texture";
	return sDummyTex;
}

//-------------------------------------------
bool SrcMesh::hasTexture(aiTextureType aTexType){
	for( auto& tex : meshTextures ){
		if( tex && tex->getTextureType() == aTexType){
			return tex->hasTexture();
		}
	}
	return false;
}

//-------------------------------------------
bool SrcMesh::hasTexture(ofMaterialTextureType aType){
	return hasTexture(_getAiTypeForOfType(aType));
}

//-------------------------------------------
ofTexture& SrcMesh::getTexture(aiTextureType aTexType){
	for( auto & tex : meshTextures ){
		if( tex && tex->getTextureType() == aTexType){
			return tex->getTextureRef();
		}
	}
	ofLogWarning("ofx::assimp::SrcMesh::getTexture : unable to find texture ref for ") << aTexType;
	return sDummyTex;
}

//-------------------------------------------
ofTexture& SrcMesh::getTexture(ofMaterialTextureType aType){
	return getTexture( _getAiTypeForOfType(aType) );
}

//-------------------------------------------
void SrcMesh::setAiMesh( aiMesh* amesh, aiNode* aAiNode ) {
	SrcNode::setAiNode(aAiNode);
	mAiMesh = amesh;
	if( mAiMesh != NULL ) {
		vbo = std::make_shared<ofVbo>();
		mName = mAiMesh->mName.data;
		getName(); // forces setting of the name
		if(!material) {
			material = std::make_shared<ofMaterial>();
		}
//		recalculateBounds(true);
		
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

//-------------------------------------------
void SrcMesh::_initTextureTypeMap() {
	if(sAiTexTypeToOfTexTypeMap.size() < 1 ) {
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_DIFFUSE] = OF_MATERIAL_TEXTURE_DIFFUSE;
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_EMISSIVE] = OF_MATERIAL_TEXTURE_EMISSIVE;
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_NORMALS] = OF_MATERIAL_TEXTURE_NORMAL;
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_LIGHTMAP] = OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC;
		//17 = aiTextureType_AMBIENT_OCCLUSION; //use this when we want to support newer assimp only
		sAiTexTypeToOfTexTypeMap[17] = OF_MATERIAL_TEXTURE_OCCLUSION;
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_AMBIENT] = OF_MATERIAL_TEXTURE_AMBIENT;
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_SPECULAR] = OF_MATERIAL_TEXTURE_SPECULAR;
		sAiTexTypeToOfTexTypeMap[15] = OF_MATERIAL_TEXTURE_METALLIC; //aiTextureType_METALNESS
		sAiTexTypeToOfTexTypeMap[16] = OF_MATERIAL_TEXTURE_ROUGHNESS; //aiTextureType_DIFFUSE_ROUGHNESS
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_DISPLACEMENT] = OF_MATERIAL_TEXTURE_DISPLACEMENT;
		sAiTexTypeToOfTexTypeMap[20] = OF_MATERIAL_TEXTURE_CLEARCOAT; // aiTextureType_CLEARCOAT = 20,
		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_NONE] = OF_MATERIAL_TEXTURE_NONE;
	}
}

//-------------------------------------------
ofMaterialTextureType SrcMesh::_getOfTypeForAiType(aiTextureType aTexType) {
	_initTextureTypeMap();
	if( sAiTexTypeToOfTexTypeMap.count(aTexType) > 0 ) {
		return sAiTexTypeToOfTexTypeMap[(int)aTexType];
	}
	return OF_MATERIAL_TEXTURE_NONE;
}

//-------------------------------------------
aiTextureType SrcMesh::_getAiTypeForOfType(ofMaterialTextureType aTexType) {
	_initTextureTypeMap();
	for( auto iter = sAiTexTypeToOfTexTypeMap.begin(); iter != sAiTexTypeToOfTexTypeMap.end(); iter++ ) {
		if( iter->second == aTexType ) {
			return (aiTextureType)iter->first;
		}
	}
	return aiTextureType_NONE;
}
