//TODO:
//handle STREAM vs STATIC draw allocation for meshes?
//animations
//bounding box/scale/etc?

#include "ofModel.h"

//--------------------------------------------------------------
ofModel::ofModel(){
	renderMethod = OF_MESH_USING_VBO;
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
	for (int i =0; i < (int)meshes.size();i++){
		
		bool goodToTexture = bUsingTextures && meshes[i]->getNumTexCoords();
		if(goodToTexture){
			bindTextureForMesh(i);
		}

		meshes[i]->drawWireframe();
		
		if(goodToTexture){
			unbindTextureForMesh(i);
		}
	}
}

//--------------------------------------------------------------
void ofModel::drawFaces(){
	for (int i =0; i < (int)meshes.size();i++){
		
		bool goodToTexture = bUsingTextures && meshes[i]->getNumTexCoords();
		if(goodToTexture){
			bindTextureForMesh(i);
		}

		meshes[i]->drawFaces();
		
		if(goodToTexture){
			unbindTextureForMesh(i);
		}
	}
}

//--------------------------------------------------------------
void ofModel::drawVertices(){
	for (int i =0; i < (int)meshes.size();i++){
		
		bool goodToTexture = bUsingTextures && meshes[i]->getNumTexCoords();
		if(goodToTexture){
			bindTextureForMesh(i);
		}

		meshes[i]->drawVertices();
		
		if(goodToTexture){
			unbindTextureForMesh(i);
		}
	}
}

//--------------------------------------------------------------
ofMesh * ofModel::getMesh(unsigned int nIndex) {
	if(nIndex < meshes.size()) {
		return meshes.at(nIndex);
	}
	return NULL;
}

//--------------------------------------------------------------
ofMesh* ofModel::getMesh(string sName) {
	map<string, ofMesh*>::iterator it = named_meshes.find(sName);
	if(it != named_meshes.end()) {
		return it->second;
	}
	return NULL; 
}

//--------------------------------------------------------------
void ofModel::listMeshNames() {
	map<string, ofMesh*>::iterator it = named_meshes.begin();
	while(it != named_meshes.end()) {
		cout << "Mesh: '" << it->first << "'" << std::endl;
		++it;
	}
}


//--------------------------------------------------------------
void ofModel::enableTextures(){
	bUsingTextures = true;
}

//--------------------------------------------------------------
void ofModel::enableNormals(){
	bUsingNormals = true;
}

//--------------------------------------------------------------
void ofModel::enableColors(){
	bUsingColors = true;
}

//--------------------------------------------------------------
void ofModel::disableTextures(){
	bUsingTextures = false;
}

//--------------------------------------------------------------
void ofModel::disableNormals(){
	bUsingNormals = false;
}

//--------------------------------------------------------------
void ofModel::disableColors(){
	bUsingColors = false;
}

//--------------------------------------------------------------
void ofModel::setRenderMethod(meshRenderMethod m){
	renderMethod = m;
}
