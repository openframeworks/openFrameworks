//
//  ofxAssimpMeshHelper.cpp
//  Created by Lukasz Karluk on 4/12/12.
//

#include "ofxAssimpMesh.h"
#include "ofxAssimpUtils.h"
#include "of3dGraphics.h"
#include "ofGraphics.h"

using std::make_shared;
using std::shared_ptr;

using namespace ofx::assimp;

//ofTexture Mesh::sDummyTex;
////static unsigned int sUniqueMeshCounter = 0;
////std::unordered_map< int, ofMaterialTextureType > Mesh::sAiTexTypeToOfTexTypeMap;
//
//
////-------------------------------------------
//void Mesh::addTexture( std::shared_ptr<ofx::assimp::Texture> aAssimpTex){
//	
//	if(!material) {
//		material = std::make_shared<ofMaterial>();
//	}
//	
////	material->setPBR(true);
//
//	auto tAiType = aAssimpTex->getTextureType();
//	auto tofType = _getOfTypeForAiType(tAiType);
//	
//	if(tofType == OF_MATERIAL_TEXTURE_NONE ) {
//		ofLogError("ofx::assimp::Mesh::addTexture") << aAssimpTex->getTextureTypeAsString();
//		return;
//	}
//	
//	bool bGoAheadAndAdd = false;
//	
//	if( tofType == OF_MATERIAL_TEXTURE_DIFFUSE) {
//		if(!hasTexture(OF_MATERIAL_TEXTURE_DIFFUSE)) {
//			bGoAheadAndAdd = true;
//		}
//	} else {
//		if( tofType != OF_MATERIAL_TEXTURE_NONE ) {
//			bGoAheadAndAdd = true;
//		}
//	}
//	
//	if(bGoAheadAndAdd) {
////		auto nAssimpTex = std::make_shared<ofxAssimpTexture>();
////		(*nAssimpTex.get()) = aAssimpTex;
//		meshTextures.push_back(aAssimpTex);
//		material->setTexture(tofType, aAssimpTex->getTextureRef());
//	}
//}
//



//-------------------------------------------
bool Mesh::hasTexture() {
	if(mSrcMesh) {
		return mSrcMesh->hasTexture();
	}
	return false;
}

//-------------------------------------------
ofTexture& Mesh::getTexture() {
	if(mSrcMesh) {
		return mSrcMesh->getTexture();
	}
	return ofx::assimp::SrcMesh::sDummyTex;
//	_initTextureTypeMap();
//	for( auto iter = sAiTexTypeToOfTexTypeMap.begin(); iter != sAiTexTypeToOfTexTypeMap.end(); iter++ ) {
//		if( hasTexture((aiTextureType)iter->first) ) {
//			return getTexture((aiTextureType)iter->first);
//		}
//	}
//	ofLogWarning("ofx::assimp::Mesh::getTexture") << " unable to find any allocated texture";
//	return sDummyTex;
}
//
////-------------------------------------------
//bool Mesh::hasTexture(aiTextureType aTexType){
//	for( auto& tex : meshTextures ){
//		if( tex && tex->getTextureType() == aTexType){
//			return tex->hasTexture();
//		}
//	}
//	return false;
//}
//
////-------------------------------------------
//bool Mesh::hasTexture(ofMaterialTextureType aType){
//	return hasTexture(_getAiTypeForOfType(aType));
//}
//
////-------------------------------------------
//ofTexture& Mesh::getTexture(aiTextureType aTexType){
//	for( auto & tex : meshTextures ){
//		if( tex && tex->getTextureType() == aTexType){
//			return tex->getTextureRef();
//		}
//	}
//	ofLogWarning("ofx::assimp::Mesh::getTexture : unable to find texture ref for ") << aTexType;
//	return sDummyTex;
//}
//
////-------------------------------------------
//ofTexture& Mesh::getTexture(ofMaterialTextureType aType){
//	return getTexture( _getAiTypeForOfType(aType) );
//}

////-------------------------------------------
//void Mesh::setup( mSrcMesh ) {
//	mAiMesh = amesh;
//	if( mAiMesh != NULL ) {
//		getName(); // forces setting of the name
//		material = std::make_shared<ofMaterial>();
//		recalculateBounds(true);
//
//		mName = mAiMesh->mName.data;
//		if(mName.empty() ) {
//			// the mesh is not named or we were unable to detect one
//			mName = "Default Mesh "+ofToString(sUniqueMeshCounter,0);
//			if( sUniqueMeshCounter > std::numeric_limits<unsigned int>().max()-3) {
//				sUniqueMeshCounter = 0;
//			}
//		}
//	}
//}

//-------------------------------------------
size_t Mesh::getNumVertices() {
	if(mSrcMesh && mSrcMesh->getAiMesh()) {
		return mSrcMesh->getAiMesh()->mNumVertices;
	}
	return 0;
}

//-------------------------------------------
bool Mesh::hasNormals() {
	if(mSrcMesh && mSrcMesh->getAiMesh()) {
		return mSrcMesh->getAiMesh()->HasNormals();
	}
	return false;
}

//-------------------------------------------
aiMesh* Mesh::getAiMesh() {
	if(mSrcMesh) {
		return mSrcMesh->getAiMesh();
	}
	return nullptr;
}

//-------------------------------------------
std::size_t Mesh::getNumIndices() {
	if(mSrcMesh) {
		return mSrcMesh->indices.size();
	}
	return 0;
}

//-------------------------------------------
void Mesh::setSrcMesh( std::shared_ptr<ofx::assimp::SrcMesh> aSrcMesh ) {
	mSrcMesh = aSrcMesh;
	
	// if we have bones, no offset, since we are controlled by the bones //
	if( mSrcMesh->getAiMesh() && mSrcMesh->getAiMesh()->mNumBones < 1 ) {
//		setOfNodeFromAiMatrix(mSrcMesh->getAiNode()->mTransformation, this );
	}
	
	if( mSrcMesh ) {
		//TODO: something fancy
		material = mSrcMesh->material;
		twoSided = mSrcMesh->twoSided;
		blendMode = mSrcMesh->blendMode;
		vbo = mSrcMesh->vbo;
		mLocalBounds = mSrcMesh->getLocalBounds();
		bConvertedToLeftHand = mSrcMesh->bConvertedToLeftHand;
	}
}

//-------------------------------------------
ofMesh& Mesh::getStaticMesh() {
	if( mSrcMesh && mSrcMesh->getAiMesh() && mMesh.getNumVertices() < 1 ) {
		if( hasTexture() ) {
			aiMeshToOfMesh(mSrcMesh->getAiMesh(), mMesh, !bConvertedToLeftHand, &getTexture() );
		} else {
			aiMeshToOfMesh(mSrcMesh->getAiMesh(), mMesh, !bConvertedToLeftHand, nullptr);
		}
	}
	return mMesh;
}

//-------------------------------------------
ofMesh& Mesh::getMesh() {
	if( animatedPos.size() > 0 ) {
		if(!validCache){
			if( !mBInitedAnimatedMesh ) {
				mBInitedAnimatedMesh=true;
				if(mSrcMesh && mSrcMesh->getAiMesh()) {
					if( hasTexture() ) {
						aiMeshToOfMesh(mSrcMesh->getAiMesh(), mAnimatedMesh, !bConvertedToLeftHand, &getTexture() );
					} else {
						aiMeshToOfMesh(mSrcMesh->getAiMesh(), mAnimatedMesh, !bConvertedToLeftHand, nullptr);
					}
				}
			}
			
			mAnimatedMesh.clearVertices();
			mAnimatedMesh.clearNormals();
			mAnimatedMesh.addVertices(aiVecVecToOfVecVec(animatedPos));
			if(animatedNorm.size() == animatedPos.size() ) {
				mAnimatedMesh.addNormals(aiVecVecToOfVecVec(animatedNorm));
			}
			validCache = true;
		}
		return mAnimatedMesh;
	}
	return getStaticMesh();
}

//-------------------------------------------
ofMesh Mesh::getTransformedStaticMesh() {
	auto srcMesh = getStaticMesh();
	glm::mat4 gmat = getGlobalTransformMatrix();
	glm::mat4 normalMat = glm::transpose(glm::inverse(gmat));
	bool bNormals = srcMesh.usingNormals() && (srcMesh.getNumNormals() == srcMesh.getNumVertices());
	for( auto& vert : srcMesh.getVertices() ) {
		vert = gmat * glm::vec4(vert.x, vert.y, vert.z, 1.0f );
	}
	if( bNormals ) {
		for( auto& normal : srcMesh.getNormals() ) {
			normal = normalMat * glm::vec4(normal.x, normal.y, normal.z, 1.0f );
		}
	}
	return srcMesh;
}

//-------------------------------------------
ofMesh Mesh::getTransformedMesh() {
	auto srcMesh = getMesh();
	glm::mat4 gmat = getGlobalTransformMatrix();
	glm::mat4 normalMat = glm::transpose(glm::inverse(gmat));
	bool bNormals = srcMesh.usingNormals() && (srcMesh.getNumNormals() == srcMesh.getNumVertices());
	for( auto& vert : srcMesh.getVertices() ) {
		vert = gmat * glm::vec4(vert.x, vert.y, vert.z, 1.0f );
	}
	if( bNormals ) {
		for( auto& normal : srcMesh.getNormals() ) {
			normal = normalMat * glm::vec4(normal.x, normal.y, normal.z, 1.0f );
		}
	}
	return srcMesh;
}

//-------------------------------------------
Bounds Mesh::getLocalBounds() {
	recalculateBounds();
	return mLocalBounds;
}

//-------------------------------------------
Bounds Mesh::getModelBounds() {
	recalculateBounds();
	return mModelBounds;
}

//-------------------------------------------
Bounds Mesh::getGlobalBounds() {
	recalculateBounds();
	return mGlobalBounds;
}

//-------------------------------------------
void Mesh::recalculateBounds(bool abForce) {
	if(hasChanged || abForce) {
		// calculate local bounds since verts have changed //
		mLocalBounds.clear();
//		mLocalBounds.include( cachedMesh.getVertices() );
		if( animatedPos.size() > 3 ) {
			mLocalBounds.include(animatedPos);
		} else {
//			mLocalBounds.include( getStaticMesh().getVertices() );
			if( mSrcMesh ) {
				mLocalBounds = mSrcMesh->getLocalBounds();
			} else {
				mLocalBounds.include( getStaticMesh().getVertices() );
			}
		}
		bBoundsDirty = true;
	}
	if(bBoundsDirty) {
		// update the model bounds
		mModelBounds.clear();
		const glm::mat4& modelMat = getLocalTransformMatrix();
		auto& tverts = mLocalBounds.getBoundingVerts();//{mLocalBounds.min, mLocalBounds.max};
		mModelBounds.include(modelMat, tverts );
		// update globally
		mGlobalBounds.clear();
		mGlobalBounds.include(getGlobalTransformMatrix(), tverts );
	}
	bBoundsDirty=false;
}

////-------------------------------------------
//void Mesh::_initTextureTypeMap() {
//	if(sAiTexTypeToOfTexTypeMap.size() < 1 ) {
//		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_DIFFUSE] = OF_MATERIAL_TEXTURE_DIFFUSE;
//		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_EMISSIVE] = OF_MATERIAL_TEXTURE_EMISSIVE;
//		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_NORMALS] = OF_MATERIAL_TEXTURE_NORMAL;
//		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_LIGHTMAP] = OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC;
//		//17 = aiTextureType_AMBIENT_OCCLUSION; //use this when we want to support newer assimp only
//		sAiTexTypeToOfTexTypeMap[17] = OF_MATERIAL_TEXTURE_OCCLUSION;
//		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_AMBIENT] = OF_MATERIAL_TEXTURE_AMBIENT;
//		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_SPECULAR] = OF_MATERIAL_TEXTURE_SPECULAR;
//		sAiTexTypeToOfTexTypeMap[15] = OF_MATERIAL_TEXTURE_METALLIC; //aiTextureType_METALNESS
//		sAiTexTypeToOfTexTypeMap[16] = OF_MATERIAL_TEXTURE_ROUGHNESS; //aiTextureType_DIFFUSE_ROUGHNESS
//		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_DISPLACEMENT] = OF_MATERIAL_TEXTURE_DISPLACEMENT;
//		sAiTexTypeToOfTexTypeMap[20] = OF_MATERIAL_TEXTURE_CLEARCOAT; // aiTextureType_CLEARCOAT = 20,
//		sAiTexTypeToOfTexTypeMap[(int)aiTextureType_NONE] = OF_MATERIAL_TEXTURE_NONE;
//	}
//}
//
////-------------------------------------------
//ofMaterialTextureType Mesh::_getOfTypeForAiType(aiTextureType aTexType) {
//	_initTextureTypeMap();
//	if( sAiTexTypeToOfTexTypeMap.count(aTexType) > 0 ) {
//		return sAiTexTypeToOfTexTypeMap[(int)aTexType];
//	}
//	return OF_MATERIAL_TEXTURE_NONE;
//}
//
////-------------------------------------------
//aiTextureType Mesh::_getAiTypeForOfType(ofMaterialTextureType aTexType) {
//	_initTextureTypeMap();
//	for( auto iter = sAiTexTypeToOfTexTypeMap.begin(); iter != sAiTexTypeToOfTexTypeMap.end(); iter++ ) {
//		if( iter->second == aTexType ) {
//			return (aiTextureType)iter->first;
//		}
//	}
//	return aiTextureType_NONE;
//}

//-------------------------------------------
void Mesh::onPositionChanged() {
	bBoundsDirty = true;
}

//-------------------------------------------
void Mesh::onOrientationChanged() {
	bBoundsDirty = true;
}

//-------------------------------------------
void Mesh::onScaleChanged() {
	bBoundsDirty = true;
}
