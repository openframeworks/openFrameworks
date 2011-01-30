//TODO:
//handle STREAM vs STATIC draw allocation for meshes
//animations
//bounding box/scale/etc?

#include "ofModel.h"

//--------------------------------------------------------------
ofModel::ofModel(){
	bUsingTextures = false;
}

//--------------------------------------------------------------
ofModel::~ofModel(){

}

//--------------------------------------------------------------
void ofModel::bindTextureForMesh(int id){
	int texId = textureLinks[id];
	if(texId!=-1){
		textures.at(texId).getTextureReference().bind();	
	}
}

//--------------------------------------------------------------
void ofModel::unbindTextureForMesh(int id){
	int texId = textureLinks[id];
	if(texId!=-1){
		textures.at(texId).getTextureReference().unbind();	
	}
}

//--------------------------------------------------------------
void ofModel::drawWireframe(){
	for (int i =0; i < meshes.size();i++){
		
		if(!meshes.at(i).bEnableIndices){
			meshes.at(i).setUseIndices();
		}
		
		if(bUsingTextures){
			bindTextureForMesh(i);
		}
		
		meshes.at(i).drawWireframe();
		
		if(bUsingTextures){
			unbindTextureForMesh(i);
		}
	}
}

//--------------------------------------------------------------
void ofModel::drawFaces(){
	for (int i =0; i < meshes.size();i++){
		
		if(!meshes.at(i).bEnableIndices){
			meshes.at(i).setUseIndices();
		}
		
		if(bUsingTextures){
			bindTextureForMesh(i);
		}
		
		meshes.at(i).drawFaces();
		
		if(bUsingTextures){
			unbindTextureForMesh(i);
		}
	}
}

//--------------------------------------------------------------
void ofModel::drawVertices(){
	for (int i =0; i < meshes.size();i++){
		
		if(!meshes.at(i).bEnableIndices){
			meshes.at(i).setUseIndices();
		}
		
		if(bUsingTextures){
			bindTextureForMesh(i);
		}
		
		meshes.at(i).drawVertices();
		
		if(bUsingTextures){
			unbindTextureForMesh(i);
		}
	}
}

//--------------------------------------------------------------
void ofModel::enableTextures(){
	bUsingTextures = true;
	for (int i =0; i < meshes.size();i++){
		meshes.at(i).enableTexCoords();
	}
}

//--------------------------------------------------------------
void ofModel::enableNormals(){
	for (int i =0; i < meshes.size();i++){
		meshes.at(i).enableNormals();
	}
}

//--------------------------------------------------------------
void ofModel::enableColors(){
	for (int i =0; i < meshes.size();i++){
		meshes.at(i).enableColors();
	}
}

//--------------------------------------------------------------
void ofModel::disableTextures(){
	bUsingTextures = false;
	for (int i =0; i < meshes.size();i++){
		meshes.at(i).disableTexCoords();
	}
}

//--------------------------------------------------------------
void ofModel::disableNormals(){
	for (int i =0; i < meshes.size();i++){
		meshes.at(i).disableNormals();
	}
}

//--------------------------------------------------------------
void ofModel::disableColors(){
	for (int i =0; i < meshes.size();i++){
		meshes.at(i).disableColors();
	}
}

//--------------------------------------------------------------
void ofModel::setRenderMethod(meshRenderMethod m){
	for (int i =0; i < meshes.size();i++){
		meshes.at(i).setRenderMethod(m);
	}
}
