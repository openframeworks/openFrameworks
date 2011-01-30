//TODO:
//handle STREAM vs STATIC draw allocation for meshes
//animations
//textures
//bounding box/scale/etc?
//incorporate loading here instead of ofModelLoader?

#include "ofModel.h"

//--------------------------------------------------------------
ofModel::ofModel(){
	renderMethod = OF_MESH_USING_DEFAULT_RENDERER;
}

//--------------------------------------------------------------
ofModel::~ofModel(){

}

//--------------------------------------------------------------
void ofModel::drawWireframe(){
	for (int i =0; i < (int)meshes.size();i++){
		
		/*if(!meshes.at(i).bEnableIndices){
			meshes.at(i).setUseIndices();
		}*/
		

		meshes.at(i).setRenderMethod(renderMethod);

		
		meshes.at(i).drawWireframe();
	}
}

//--------------------------------------------------------------
void ofModel::drawFaces(){
	for (int i =0; i < (int)meshes.size();i++){
		
		/*if(!meshes.at(i).bEnableIndices){
			meshes.at(i).setUseIndices();
		}*/
		

		meshes.at(i).setRenderMethod(renderMethod);

		
		meshes.at(i).drawFaces();
	}
}

//--------------------------------------------------------------
void ofModel::drawVertices(){
	for (int i =0; i < (int)meshes.size();i++){
		
		/*if(!meshes.at(i).bEnableIndices){
			meshes.at(i).setUseIndices();
		}*/
		

		meshes.at(i).setRenderMethod(renderMethod);

		
		meshes.at(i).drawVertices();
	}
}

//--------------------------------------------------------------
void ofModel::enableTexCoords(){
	for (int i =0; i < (int)meshes.size();i++){
		meshes.at(i).enableTexCoords();
	}
}

//--------------------------------------------------------------
void ofModel::enableNormals(){
	for (int i =0; i < (int)meshes.size();i++){
		meshes.at(i).enableNormals();
	}
}

//--------------------------------------------------------------
void ofModel::enableColors(){
	for (int i =0; i < (int)meshes.size();i++){
		meshes.at(i).enableColors();
	}
}

//--------------------------------------------------------------
void ofModel::disableTexCoords(){
	for (int i =0; i < (int)meshes.size();i++){
		meshes.at(i).enableTexCoords();
	}
}

//--------------------------------------------------------------
void ofModel::disableNormals(){
	for (int i =0; i < (int)meshes.size();i++){
		meshes.at(i).enableNormals();
	}
}

//--------------------------------------------------------------
void ofModel::disableColors(){
	for (int i =0; i < (int)meshes.size();i++){
		meshes.at(i).enableColors();
	}
}

//--------------------------------------------------------------
void ofModel::setRenderMethod(meshRenderMethod m){
	renderMethod = m;
}
