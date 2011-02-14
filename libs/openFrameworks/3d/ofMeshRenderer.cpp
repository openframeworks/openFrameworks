//TODO: individual enabling/disabling (and getters) per-vbo instead of hacky placeholder lump-sum method currently in place
//TODO: better way to organize vbos in relation to different meshes

#include "ofMeshRenderer.h"
#include "ofGraphics.h"

//--------------------------------------------------------------
ofMeshRenderer::ofMeshRenderer(){
	mesh = NULL;
	renderMethod = OF_MESH_USING_DEFAULT_RENDERER;
	drawType = GL_STATIC_DRAW;
	bEnableIndices = false;
	bEnableColors = false;
	bEnableTexCoords = false;
	bEnableNormals = false;
}

//--------------------------------------------------------------
ofMeshRenderer::~ofMeshRenderer(){
}

//--------------------------------------------------------------
void ofMeshRenderer::setDrawType(int drawType_){
	drawType = drawType_;
}

//--------------------------------------------------------------
void ofMeshRenderer::setRenderMethod(meshRenderMethod m){
	renderMethod = m;
}

/*
//--------------------------------------------------------------
ofMeshRenderer::ofMeshRenderer(const ofMeshRenderer& v){
	clone(v);
}

//--------------------------------------------------------------
ofMeshRenderer& ofMeshRenderer::operator=(const ofMeshRenderer& v){
	clone(v);
	return *this;
}

//--------------------------------------------------------------
void ofMeshRenderer::clone(const ofMeshRenderer& v){
	mesh->primitives.at(i) = v.getmesh->primitives.at(i)();
	vbos = v.vbos;
}
 */

//--------------------------------------------------------------
void ofMeshRenderer::setMesh(ofMesh* m){
	mesh = m;
}

//--------------------------------------------------------------
ofMesh* ofMeshRenderer::getMesh(){
	return mesh;
}

//--------------------------------------------------------------
const ofMesh* ofMeshRenderer::getMesh() const{
	return mesh;
}

//--------------------------------------------------------------
bool ofMeshRenderer::setupVerticesForVbo(int id, int usage){
	int size = mesh->primitives.at(id).getNumVertices();
	if(size){
		vbos.at(id).setVertexData(mesh->primitives.at(id).getVerticesPointer(),3,size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofMeshRenderer:setupVertices - no vertices in mesh primitive " + ofToString(id) +".");
		return false;
	}
}

//--------------------------------------------------------------
bool ofMeshRenderer::setupColorsForVbo(int id, int usage){
	int size = mesh->primitives.at(id).getNumColors();
	if(size){
		vbos.at(id).setColorData(mesh->primitives.at(id).getColorsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofMeshRenderer:setupColors - no colors in mesh primitive " + ofToString(id) +".");
		return false;
	}
}

//--------------------------------------------------------------
bool ofMeshRenderer::setupNormalsForVbo(int id, int usage){
	int size = mesh->primitives.at(id).getNumNormals();
	if(size){
		vbos.at(id).setNormalData(mesh->primitives.at(id).getNormalsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofMeshRenderer:setupNormals - no normals in mesh primitive " + ofToString(id) +".");
		return false;
	}
}

//--------------------------------------------------------------
bool ofMeshRenderer::setupTexCoordsForVbo(int id, int usage){
	int size = mesh->primitives.at(id).getNumTexCoords();
	if(size){
		vbos.at(id).setTexCoordData(mesh->primitives.at(id).getTexCoordsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofMeshRenderer:setupTexCoords - no texCoords in mesh primitive " + ofToString(id) +".");
		return false;
	}
}

//--------------------------------------------------------------
bool ofMeshRenderer::setupIndicesForVbo(int id, int usage){
	int size = mesh->primitives.at(id).getNumIndices();
	if(size){
		vbos.at(id).setIndexData(mesh->primitives.at(id).getIndexPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofMeshRenderer:setupIndices - no indices in mesh primitive " + ofToString(id) +".");
		return false;
	}
}

//--------------------------------------------------------------
bool ofMeshRenderer::enableColors(){
	for (int i =0; i < (int)mesh->primitives.size();i++){
		if(!mesh->primitives.at(i).getNumColors()){
			bEnableColors = false;
			return false;
		}
	}
	bEnableColors = true;
	return true;
}

//--------------------------------------------------------------
void ofMeshRenderer::disableColors(){
	bEnableColors = false;
}

//--------------------------------------------------------------
bool ofMeshRenderer::enableNormals(){
	for (int i =0; i < (int)mesh->primitives.size();i++){
		if(!mesh->primitives.at(i).getNumNormals()){
			bEnableNormals = false;
			return false;
		}
	}
	bEnableNormals = true;
	return true;
}

//--------------------------------------------------------------
void ofMeshRenderer::disableNormals(){
	bEnableNormals = false;
}

//--------------------------------------------------------------
bool ofMeshRenderer::enableTexCoords(){
	for (int i =0; i < (int)mesh->primitives.size();i++){
		if(!mesh->primitives.at(i).getNumTexCoords()){
			bEnableTexCoords = false;
			return false;
		}
	}
	bEnableTexCoords = true;
	return true;
}

//--------------------------------------------------------------
void ofMeshRenderer::disableTexCoords(){
	bEnableTexCoords = false;
}

//--------------------------------------------------------------
bool ofMeshRenderer::enableIndices(){
	for (int i =0; i < (int)mesh->primitives.size();i++){
		if(!mesh->primitives.at(i).getNumIndices()){
			bEnableIndices = false;
			return false;
		}
	}
	bEnableIndices = true;
	return true;
}

//--------------------------------------------------------------
void ofMeshRenderer::disableIndices(){
	bEnableIndices = false;
}

//--------------------------------------------------------------
bool ofMeshRenderer::getTexCoordsEnabled(){
	for (int i =0; i < (int)mesh->primitives.size();i++){
		if(!mesh->primitives.at(i).getNumTexCoords()){
			bEnableTexCoords = false;
		}
	}
	return bEnableTexCoords;
}

//--------------------------------------------------------------
bool ofMeshRenderer::getNormalsEnabled(){
	for (int i =0; i < (int)mesh->primitives.size();i++){
		if(!mesh->primitives.at(i).getNumNormals()){
			bEnableNormals = false;
		}
	}
	return bEnableNormals;
}

//--------------------------------------------------------------
bool ofMeshRenderer::getColorsEnabled(){
	for (int i =0; i < (int)mesh->primitives.size();i++){
		if(!mesh->primitives.at(i).getNumColors()){
			bEnableColors = false;
		}
	}
	return bEnableColors;
}
	
//--------------------------------------------------------------
bool ofMeshRenderer::getIndicesEnabled(){
	for (int i =0; i < (int)mesh->primitives.size();i++){
		if(!mesh->primitives.at(i).getNumIndices()){
			bEnableIndices = false;
		}
	}
	return bEnableIndices;
}

//--------------------------------------------------------------
void ofMeshRenderer::setupVbos(){
	for (int i =0; i < (int)vbos.size();i++){
		if(mesh->primitives.at(i).getNumVertices()){
			if(!vbos.at(i).getIsAllocated()){
				setupVerticesForVbo(i,drawType);
			}
			
			if (mesh->primitives.at(i).haveVertsChanged()){
				vbos.at(i).updateVertexData(mesh->primitives.at(i).getVerticesPointer(), 3, mesh->primitives.at(i).getNumVertices());
			}
		}
		
		if(getColorsEnabled()){
			if(!vbos.at(i).getUsingColors()){
				setupColorsForVbo(i,drawType);
			}
		
			if(mesh->primitives.at(i).haveColorsChanged()){
				vbos.at(i).updateColorData(mesh->primitives.at(i).getColorsPointer(), mesh->primitives.at(i).getNumColors());
			}
		}
		
		if(getNormalsEnabled()){
			if(!vbos.at(i).getUsingNormals()){
				setupNormalsForVbo(i,drawType);
			}
		
			if(mesh->primitives.at(i).haveNormalsChanged()){
				vbos.at(i).updateNormalData(mesh->primitives.at(i).getNormalsPointer(), mesh->primitives.at(i).getNumNormals());
			}
		}
		
		if(getTexCoordsEnabled()){
			if(!vbos.at(i).getUsingTexCoords()){
				setupTexCoordsForVbo(i,drawType);
			}
		
			if(mesh->primitives.at(i).haveTexCoordsChanged()){
				vbos.at(i).updateTexCoordData(mesh->primitives.at(i).getTexCoordsPointer(), mesh->primitives.at(i).getNumTexCoords());
			}
		}
		
		if(getIndicesEnabled()){
			if(!vbos.at(i).getUsingIndices()){
				setupIndicesForVbo(i,drawType);
			}
		
			if(mesh->primitives.at(i).haveIndicesChanged()){
				vbos.at(i).updateIndexData(mesh->primitives.at(i).getIndexPointer(), mesh->primitives.at(i).getNumIndices());
			}
		}	
	}
}

//--------------------------------------------------------------
void ofMeshRenderer::draw(ofMesh* meshToDraw, polyMode renderType){
	if(meshToDraw){
		if (mesh!=meshToDraw){
			mesh = meshToDraw;
		}
		
		if(vbos.size()!=mesh->primitives.size()){
			//take care of changes
			vbos.resize(mesh->primitives.size());
			for (int i =0; i < (int)vbos.size();i++){
				vbos.at(i).clear();
			}
		}
			
		if(renderMethod == OF_MESH_USING_VBO){
			setupVbos();
		}
	
#ifndef TARGET_OPENGLES 
		glPushAttrib(GL_POLYGON_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
#endif
	
		for (int i = 0; i < (int)vbos.size();i++){
			if(renderMethod == OF_MESH_USING_VBO){
				GLuint mode = ofGetGLPrimitiveMode(mesh->primitives.at(i).getMode());
				if(getIndicesEnabled()){
					vbos.at(i).drawElements(mode,mesh->primitives.at(i).getNumIndices());
				}else{
					vbos.at(i).draw(mode,0,mesh->primitives.at(i).getNumVertices());
				}
			}else{
				ofGetDefaultRenderer()->draw(mesh->primitives.at(i));
			}
		}
		
#ifndef TARGET_OPENGLES
		glPopAttrib();
#endif
	}
}

//--------------------------------------------------------------
void ofMeshRenderer::drawVertices(ofMesh* meshToDraw){
	draw(meshToDraw, OF_MESH_POINTS);
}

//--------------------------------------------------------------
void ofMeshRenderer::drawWireframe(ofMesh* meshToDraw){
	draw(meshToDraw, OF_MESH_WIREFRAME);
}

//--------------------------------------------------------------
void ofMeshRenderer::drawFaces(ofMesh* meshToDraw){
	draw(meshToDraw, OF_MESH_FILL);
}
