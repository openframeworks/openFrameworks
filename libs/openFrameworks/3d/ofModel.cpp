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
void ofModel::updateRenderers(){
	renderers.resize(meshes.size());
	for (int i =0; i < (int)renderers.size();i++){
		renderers[i]=ofMeshRenderer();
		renderers[i].setMesh(meshes[i]);
		if(bUsingTextures) renderers[i].enableTexCoords();
		if(bUsingNormals) renderers[i].enableNormals();
		if(bUsingColors) renderers[i].enableColors();
	}	
}
	
//--------------------------------------------------------------
void ofModel::drawWireframe(){
	if (renderers.size()!=meshes.size()){
		updateRenderers();
	}
	
	for (int i =0; i < (int)renderers.size();i++){
		
		if(!renderers.at(i).enableIndices()){
			ofLog(OF_LOG_WARNING, "no indices in mesh " + ofToString(i));
		}

		bool goodToTexture = bUsingTextures && renderers.at(i).getTexCoordsEnabled();
		if(goodToTexture){
			bindTextureForMesh(i);
		}

		renderers.at(i).setRenderMethod(renderMethod);
		
		renderers.at(i).drawWireframe(meshes.at(i));
		
		if(goodToTexture){
			unbindTextureForMesh(i);
		}
	}
}

//--------------------------------------------------------------
void ofModel::drawFaces(){
	if (renderers.size()!=meshes.size()){
		updateRenderers();
	}
	
	for (int i =0; i < (int)meshes.size();i++){
		if(!renderers.at(i).enableIndices()){
			ofLog(OF_LOG_WARNING, "no indices in mesh " + ofToString(i));
		}
		
		bool goodToTexture = bUsingTextures && renderers.at(i).getTexCoordsEnabled();
		if(goodToTexture){
			bindTextureForMesh(i);
		}
		renderers.at(i).setRenderMethod(renderMethod);
		
		renderers.at(i).drawFaces(meshes.at(i));
		
		if(goodToTexture){
			unbindTextureForMesh(i);
		}
	}
}

//--------------------------------------------------------------
void ofModel::drawVertices(){
	if (renderers.size()!=meshes.size()){
		updateRenderers();
	}
	
	for (int i =0; i < (int)renderers.size();i++){
		
		bool goodToTexture = bUsingTextures && renderers.at(i).getTexCoordsEnabled();
		if(goodToTexture){
			bindTextureForMesh(i);
		}

		renderers.at(i).setRenderMethod(renderMethod);
		
		renderers.at(i).drawVertices(meshes.at(i));
		
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
	for (int i =0; i < (int)renderers.size();i++){
		renderers.at(i).enableTexCoords();
	}
}

//--------------------------------------------------------------
void ofModel::enableNormals(){
	bUsingNormals = true;
	for (int i =0; i < (int)renderers.size();i++){
		renderers.at(i).enableNormals();
	}
}

//--------------------------------------------------------------
void ofModel::enableColors(){
	bUsingColors = true;
	for (int i =0; i < (int)renderers.size();i++){
		renderers.at(i).enableColors();
	}
}

//--------------------------------------------------------------
void ofModel::disableTextures(){
	bUsingTextures = false;
	for (int i =0; i < (int)renderers.size();i++){
		renderers.at(i).disableTexCoords();
	}
}

//--------------------------------------------------------------
void ofModel::disableNormals(){
	bUsingNormals = false;
	for (int i =0; i < (int)renderers.size();i++){
		renderers.at(i).disableNormals();
	}
}

//--------------------------------------------------------------
void ofModel::disableColors(){
	bUsingColors = false;
	for (int i =0; i < (int)renderers.size();i++){
		renderers.at(i).disableColors();
	}
}

//--------------------------------------------------------------
void ofModel::setRenderMethod(meshRenderMethod m){
	renderMethod = m;
}
