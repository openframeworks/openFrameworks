//TODO:
//handle STREAM vs STATIC draw allocation for meshes
//animations
//textures
//bounding box/scale/etc?
//incorporate loading here instead of ofModelLoader?

#include "ofModel.h"

//--------------------------------------------------------------
ofModel::ofModel(){

}

//--------------------------------------------------------------
ofModel::~ofModel(){

}

//--------------------------------------------------------------
void ofModel::drawWireframe(){
	for (int i =0; i < meshes.size();i++){
		
		/*if(!meshes.at(i).bEnableIndices){
			meshes.at(i).setUseIndices();
		}*/
		
		/*
		if(meshes.at(i).renderMethod!=OF_MESH_USING_VBO){
			meshes.at(i).setRenderMethod(OF_MESH_USING_VBO);
		}
		 */
		
		meshes.at(i).drawWireframe();
	}
}

//--------------------------------------------------------------
void ofModel::drawFaces(){
	for (int i =0; i < meshes.size();i++){
		
		/*if(!meshes.at(i).bEnableIndices){
			meshes.at(i).setUseIndices();
		}*/
		
		/*
		if(meshes.at(i).renderMethod!=OF_MESH_USING_VBO){
			meshes.at(i).setRenderMethod(OF_MESH_USING_VBO);
		}
		 */
		
		meshes.at(i).drawFaces();
	}
}

//--------------------------------------------------------------
void ofModel::drawVertices(){
	for (int i =0; i < meshes.size();i++){
		
		/*if(!meshes.at(i).bEnableIndices){
			meshes.at(i).setUseIndices();
		}*/
		
		/*
		if(meshes.at(i).renderMethod!=OF_MESH_USING_VBO){
			meshes.at(i).setRenderMethod(OF_MESH_USING_VBO);
		}
		 */
		
		meshes.at(i).drawVertices();
	}
}

//--------------------------------------------------------------
void ofModel::enableTexCoords(){
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
void ofModel::disableTexCoords(){
	for (int i =0; i < meshes.size();i++){
		meshes.at(i).enableTexCoords();
	}
}

//--------------------------------------------------------------
void ofModel::disableNormals(){
	for (int i =0; i < meshes.size();i++){
		meshes.at(i).enableNormals();
	}
}

//--------------------------------------------------------------
void ofModel::disableColors(){
	for (int i =0; i < meshes.size();i++){
		meshes.at(i).enableColors();
	}
}

//--------------------------------------------------------------
void ofModel::setRenderMethod(meshRenderMethod m){
	for (int i =0; i < meshes.size();i++){
		meshes.at(i).setRenderMethod(m);
	}
}
