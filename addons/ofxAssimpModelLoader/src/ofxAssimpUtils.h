//
//  ofxAssimpUtils.h
//  Created by Lukasz Karluk on 4/12/12.
//
//

#pragma once

#include "ofxAssimpMeshHelper.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
inline vector<ofDefaultVec3> aiVecVecToOfVecVec(const vector<aiVector3D>& v){
	vector<ofDefaultVec3> ofv(v.size());
	memcpy(ofv.data(),v.data(),v.size()*sizeof(ofDefaultVec3));
	return ofv;
}

//--------------------------------------------------------------
inline void aiMeshToOfMesh(const aiMesh* aim, ofMesh& ofm, ofxAssimpMeshHelper * helper = NULL){

	// default to triangle mode
	ofm.setMode(OF_PRIMITIVE_TRIANGLES);

	// copy vertices
	for (int i=0; i < (int)aim->mNumVertices;i++){
		ofm.addVertex(glm::vec3(aim->mVertices[i].x,aim->mVertices[i].y,aim->mVertices[i].z));
	}

	if(aim->HasNormals()){
		for (int i=0; i < (int)aim->mNumVertices;i++){
			ofm.addNormal(glm::vec3(aim->mNormals[i].x,aim->mNormals[i].y,aim->mNormals[i].z));
		}
	}

	// aiVector3D * 	mTextureCoords [AI_MAX_NUMBER_OF_TEXTURECOORDS]
	// just one for now
	if(aim->GetNumUVChannels()>0){
		for (int i=0; i < (int)aim->mNumVertices;i++){
			if(helper && helper->hasTexture()){
				ofTexture & tex = helper->getTextureRef();
				glm::vec2 texCoord = tex.getCoordFromPercent(aim->mTextureCoords[0][i].x, aim->mTextureCoords[0][i].y);
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
		for (int i=0; i < (int)aim->mNumVertices;i++){
			ofm.addColor(aiColorToOfColor(aim->mColors[0][i]));
		}
	}

	for (int i=0; i <(int) aim->mNumFaces;i++){
		if(aim->mFaces[i].mNumIndices>3){
			ofLogWarning("ofxAssimpUtils") << "aiMeshToOfMesh(): non triangular face found: model face " << i;
		}
		for (int j=0; j<(int)aim->mFaces[i].mNumIndices; j++){
			ofm.addIndex(aim->mFaces[i].mIndices[j]);
		}
	}
}
