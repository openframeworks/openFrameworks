//
//  ofxAssimpUtils.h
//  Created by Lukasz Karluk on 4/12/12.
//
//

#pragma once

#include "ofMain.h"
#include "ofxAssimpMeshHelper.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//--------------------------------------------------------------
inline ofFloatColor aiColorToOfColor(const aiColor4D& c){
	return ofFloatColor(c.r,c.g,c.b,c.a);
}

//--------------------------------------------------------------
inline ofFloatColor aiColorToOfColor(const aiColor3D& c){
	return ofFloatColor(c.r,c.g,c.b,1);
}

//--------------------------------------------------------------
inline ofVec3f aiVecToOfVec(const aiVector3D& v){
	return ofVec3f(v.x,v.y,v.z);
}

//--------------------------------------------------------------
inline vector<ofVec3f> aiVecVecToOfVecVec(const vector<aiVector3D>& v){
	vector<ofVec3f> ofv(v.size());
	if(sizeof(aiVector3D)==sizeof(ofVec3f)){
		memcpy(&ofv[0],&v[0],v.size()*sizeof(ofVec3f));
	}else{
		for(int i=0;i<(int)v.size();i++){
			ofv[i]=aiVecToOfVec(v[i]);
		}
	}
	return ofv;
}

//--------------------------------------------------------------
inline void aiMeshToOfMesh(const aiMesh* aim, ofMesh& ofm, ofxAssimpMeshHelper * helper = NULL){

	// default to triangle mode
	ofm.setMode(OF_PRIMITIVE_TRIANGLES);

	// copy vertices
	for (int i=0; i < (int)aim->mNumVertices;i++){
		ofm.addVertex(ofVec3f(aim->mVertices[i].x,aim->mVertices[i].y,aim->mVertices[i].z));
	}

	if(aim->HasNormals()){
		for (int i=0; i < (int)aim->mNumVertices;i++){
			ofm.addNormal(ofVec3f(aim->mNormals[i].x,aim->mNormals[i].y,aim->mNormals[i].z));
		}
	}

	// aiVector3D * 	mTextureCoords [AI_MAX_NUMBER_OF_TEXTURECOORDS]
	// just one for now
	if(aim->GetNumUVChannels()>0){
		for (int i=0; i < (int)aim->mNumVertices;i++){
			if(helper && helper->hasTexture()){
				ofTexture & tex = helper->getTextureRef();
				ofVec2f texCoord = tex.getCoordFromPercent(aim->mTextureCoords[0][i].x ,aim->mTextureCoords[0][i].y);
				ofm.addTexCoord(texCoord);
			}else{
				ofm.addTexCoord(ofVec2f(aim->mTextureCoords[0][i].x ,aim->mTextureCoords[0][i].y));
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
