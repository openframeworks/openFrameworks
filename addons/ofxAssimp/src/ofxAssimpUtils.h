//
//  ofxAssimpUtils.h
//  Created by Lukasz Karluk on 4/12/12.
//
//

#pragma once

#include "ofxAssimpMesh.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ofNode.h"

using std::string;
using std::vector;

//--------------------------------------------------------------
inline ofFloatColor aiColorToOfColor(const aiColor4D& c){
	return ofFloatColor(c.r,c.g,c.b,c.a);
}

//--------------------------------------------------------------
inline ofFloatColor aiColorToOfColor(const aiColor3D& c){
	return ofFloatColor(c.r,c.g,c.b,1);
}

//--------------------------------------------------------------
inline ofDefaultVec3 aiVecToOfVec(const aiVector3D& v){
	return ofDefaultVec3(v.x,v.y,v.z);
}

//--------------------------------------------------------------
inline void aiVecToOfVec(const aiVector3D& v, glm::vec3& fv){
	fv = glm::vec3(v.x,v.y,v.z);
}

//--------------------------------------------------------------
inline glm::quat aiQuatToOfQuat( const aiQuaternion& aq ) {
	return glm::quat(aq.w, aq.x, aq.y, aq.z);
}

//--------------------------------------------------------------
inline vector<ofDefaultVec3> aiVecVecToOfVecVec(const vector<aiVector3D>& v){
	vector<ofDefaultVec3> ofv(v.size());
	memcpy(ofv.data(),v.data(),v.size()*sizeof(ofDefaultVec3));
	return ofv;
}

inline static aiMatrix4x4 glmMat4ToAiMatrix4x4(const glm::mat4& aMat4) {
	// hmm, this didn't work, so I guess it's the other way :)
//	aiMatrix4x4 aMat4(gMat[0][0], aMat4[0][1], aMat4[0][2], aMat4[0][3],
//								 aMat4[1][0], aMat4[1][1], aMat4[1][2], aMat4[1][3],
//								 aMat4[2][0], aMat4[2][1], aMat4[2][2], aMat4[2][3],
//								 aMat4[3][0], aMat4[3][1], aMat4[3][2], aMat4[3][3]
//								 );
	
	return aiMatrix4x4(aMat4[0][0], aMat4[1][0], aMat4[2][0], aMat4[3][0],
					   aMat4[0][1], aMat4[1][1], aMat4[2][1], aMat4[3][1],
					   aMat4[0][2], aMat4[1][2], aMat4[2][2], aMat4[3][2],
					   aMat4[0][3], aMat4[1][3], aMat4[2][3], aMat4[3][3]
					   );
}

inline static glm::mat4 aiMatrix4x4ToGlmMatrix(const aiMatrix4x4& amat ) {
//	glm::mat4 tmatrix(amat.a1, amat.a2, amat.a3, amat.a4,
//					   amat.b1, amat.b2, amat.b3, amat.b4,
//					   amat.c1, amat.c2, amat.c3, amat.c4,
//					   amat.d1, amat.d2, amat.d3, amat.d4);
	glm::mat4 tmatrix(amat.a1, amat.b1, amat.c1, amat.d1,
					  amat.a2, amat.b2, amat.c2, amat.d2,
					  amat.a3, amat.b3, amat.c3, amat.d3,
					  amat.a4, amat.b4, amat.c4, amat.d4);
	return tmatrix;
}

inline void getPositionOrientationScaleFromAiMatrix(aiMatrix4x4& aMat, glm::vec3& apos, glm::quat& aq, glm::vec3& ascale ) {
	aiVector3t<float> tAiScale;
	aiQuaterniont<float> tAiRotation;
	aiVector3t<float> tAiPosition;
	
	aMat.Decompose( tAiScale, tAiRotation, tAiPosition );
	
	apos = glm::vec3( tAiPosition.x, tAiPosition.y, tAiPosition.z );
	aq = glm::quat(tAiRotation.w, tAiRotation.x, tAiRotation.y, tAiRotation.z);
	ascale = glm::vec3( tAiScale.x, tAiScale.y, tAiScale.z );
}

inline void setOfNodeFromAiMatrix(aiMatrix4x4& aMat, ofNode* anode) {
	glm::vec3 tpos, tscale;
	glm::quat tquat;
	getPositionOrientationScaleFromAiMatrix(aMat, tpos, tquat, tscale );
	anode->setPositionOrientationScale( tpos, tquat, tscale );
}

//--------------------------------------------------------------
inline void aiMeshToOfMesh(const aiMesh* aim, ofMesh& ofm, bool bInvertYTexCoord, ofTexture* aTex=nullptr){

	// default to triangle mode
	ofm.clear();
	ofm.setMode(OF_PRIMITIVE_TRIANGLES);

	// copy vertices
	for (unsigned int i=0; i < aim->mNumVertices;i++){
		ofm.addVertex(glm::vec3(aim->mVertices[i].x,aim->mVertices[i].y,aim->mVertices[i].z));
	}

	if(aim->HasNormals()){
		for (unsigned int i=0; i < aim->mNumVertices;i++){
			ofm.addNormal(glm::vec3(aim->mNormals[i].x,aim->mNormals[i].y,aim->mNormals[i].z));
		}
	}

	// aiVector3D * 	mTextureCoords [AI_MAX_NUMBER_OF_TEXTURECOORDS]
	// just one for now
	if(aim->GetNumUVChannels()>0){
		for (unsigned int i=0; i < aim->mNumVertices;i++){
//			if(helper && helper->hasTexture()){
			if( aTex && aTex->isAllocated() ) {
//				ofTexture& tex = helper->getTexture();
				glm::vec2 texCoord = aTex->getCoordFromPercent(aim->mTextureCoords[0][i].x, aim->mTextureCoords[0][i].y);
				if(bInvertYTexCoord) {
					texCoord.y = aTex->getCoordFromPercent(0.0f, 1.0f).y - texCoord.y;
				}
				ofm.addTexCoord(texCoord);
			}else{
				glm::vec2 texCoord(aim->mTextureCoords[0][i].x, aim->mTextureCoords[0][i].y);
				ofm.addTexCoord(texCoord);
			}
		}
	}

	//aiColor4D * 	mColors [AI_MAX_NUMBER_OF_COLOR_SETS]
	// just one for now
	if(aim->GetNumColorChannels()>0){
		for (unsigned int i=0; i < aim->mNumVertices;i++){
			ofm.addColor(aiColorToOfColor(aim->mColors[0][i]));
		}
	}
	
//	meshHelper->indices.resize(mesh->mNumFaces * 3);
//	int j=0;
//	for (unsigned int x = 0; x < mesh->mNumFaces; ++x){
//		for (unsigned int a = 0; a < mesh->mFaces[x].mNumIndices; ++a){
//			meshHelper->indices[j++]=mesh->mFaces[x].mIndices[a];
//		}
//	}
	
	int j=0;
	auto& indices = ofm.getIndices();
	indices.resize(aim->mNumFaces * 3);
	for (unsigned int x = 0; x < aim->mNumFaces; ++x){
		for (unsigned int a = 0; a < aim->mFaces[x].mNumIndices; ++a){
			indices[j++] = aim->mFaces[x].mIndices[a];
		}
	}

//	for (unsigned int i=0; i < aim->mNumFaces;i++){
//		if(aim->mFaces[i].mNumIndices>3){
//			ofLogWarning("ofxAssimpUtils") << "aiMeshToOfMesh(): non triangular face found: model face " << i;
//		}
//		for (unsigned int j=0; j < aim->mFaces[i].mNumIndices; j++){
//			ofm.addIndex(aim->mFaces[i].mIndices[j]);
//		}
//	}
}
