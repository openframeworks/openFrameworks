#include "ofMesh.h"

//--------------------------------------------------------------
ofMesh::ofMesh(){
	vbo = ofVbo();
	vertexData = NULL;
	drawType = GL_STATIC_DRAW_ARB;
	bEnableIndices = false;
	bEnableColors = false;
	bEnableTexCoords = false;
	bEnableNormals = false;
	renderMethod = OF_MESH_USING_VERTEX_ARRAY;
}

//--------------------------------------------------------------
ofMesh::~ofMesh(){
}

//--------------------------------------------------------------
void ofMesh::setUseIndices(bool useIndices){
	bEnableIndices = useIndices;
}

//--------------------------------------------------------------
void ofMesh::setDrawType(int drawType_){
	drawType = drawType_;
}

//--------------------------------------------------------------
void ofMesh::setRenderMethod(meshRenderMethod m){
	renderMethod = m;
}

/*
//--------------------------------------------------------------
ofMesh::ofMesh(const ofMesh& v){
	clone(v);
}

//--------------------------------------------------------------
ofMesh& ofMesh::operator=(const ofMesh& v){
	clone(v);
	return *this;
}

//--------------------------------------------------------------
void ofMesh::clone(const ofMesh& v){
	vertexData = v.getvertexData();
	vbo = v.vbo;
}
 */

//--------------------------------------------------------------
void ofMesh::setVertexData(ofVertexData* m){
	vertexData = m;
}

//--------------------------------------------------------------
ofVertexData* ofMesh::getVertexData(){
	return vertexData;
}

//--------------------------------------------------------------
const ofVertexData* ofMesh::getVertexData() const{
	return vertexData;
}

//--------------------------------------------------------------
bool ofMesh::setupVerticesForVbo(int usage){
	int size = vertexData->getNumVertices();
	if(size){
		vbo.setVertexData(vertexData->getVerticesPointer(),size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofMesh:setupVertices - no vertices in vertexData.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::setupColorsForVbo(int usage){
	int size = vertexData->getNumColors();
	if(size){
		vbo.setColorData(vertexData->getColorsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofMesh:setupColors - no colors in vertexData.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::setupNormalsForVbo(int usage){
	int size = vertexData->getNumNormals();
	if(size){
		vbo.setNormalData(vertexData->getNormalsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofMesh:setupNormals - no normals in vertexData.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::setupTexCoordsForVbo(int usage){
	int size = vertexData->getNumTexCoords();
	if(size){
		vbo.setTexCoordData(vertexData->getTexCoordsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofMesh:setupTexCoords - no texCoords in vertexData.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::setupIndicesForVbo(int usage){
	int size = vertexData->getNumIndices();
	if(size){
		vbo.setIndexData(vertexData->getIndexPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofMesh:setupIndices - no indices in vertexData.");
		return false;
	}
}

//--------------------------------------------------------------
void ofMesh::enableColors(){
	bEnableColors = true;
}

//--------------------------------------------------------------
void ofMesh::disableColors(){
	bEnableColors = false;
}

//--------------------------------------------------------------
void ofMesh::enableNormals(){
	bEnableNormals = true;
}

//--------------------------------------------------------------
void ofMesh::disableNormals(){
	bEnableNormals = false;
}

//--------------------------------------------------------------
void ofMesh::enableTexCoords(){
	bEnableTexCoords = true;
}

//--------------------------------------------------------------
void ofMesh::disableTexCoords(){
	bEnableTexCoords = false;
}

//--------------------------------------------------------------
void ofMesh::setupVbo(){	
	if(!vbo.getIsAllocated() || vertexData->haveVertsChanged()){
		setupVerticesForVbo(drawType);
	}
	
	if(bEnableColors){
		if((vertexData->getNumColors() && !vbo.getUsingColors()) || vertexData->haveColorsChanged()){
			setupColorsForVbo(drawType);
		}
	}
	
	if(bEnableNormals){
		if((vertexData->getNumNormals() && !vbo.getUsingNormals()) || vertexData->haveNormalsChanged()){
			setupNormalsForVbo(drawType);
		}
	}
	
	if(bEnableTexCoords){
		if((vertexData->getNumTexCoords() && !vbo.getUsingTexCoords()) || vertexData->haveTexCoordsChanged()){
			setupTexCoordsForVbo(drawType);
		}
	}
	
	if(bEnableIndices){
		if((vertexData->getNumIndices() && !vbo.getUsingIndices()) || vertexData->haveIndicesChanged()){
			setupIndicesForVbo(drawType);
		}
	}	
}

//--------------------------------------------------------------
void ofMesh::setupVertexArray(){
	if(vertexData->getNumVertices()){
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), vertexData->getVerticesPointer());
	}
	
	if(bEnableColors && vertexData->getNumColors()){
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, sizeof(ofColor), vertexData->getColorsPointer());
	}
	
	if(bEnableNormals && vertexData->getNumNormals()){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(ofVec3f), vertexData->getNormalsPointer());
	}
	
	if(bEnableTexCoords && vertexData->getNumTexCoords()){
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(ofVec2f), vertexData->getTexCoordsPointer());
	}

}

//--------------------------------------------------------------
void ofMesh::draw(polyMode renderType){
	if(renderMethod == OF_MESH_USING_VBO){
		setupVbo();
	}else if (renderMethod == OF_MESH_USING_VERTEX_ARRAY){
		setupVertexArray();
	}
	
	glPushAttrib(GL_POLYGON_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
	
	GLuint mode = ofGetGLPrimitiveMode(vertexData->getMode());
	
	if(bEnableIndices){
		if(renderMethod == OF_MESH_USING_VBO){
			vbo.drawElements(mode,vertexData->getNumIndices());
		}else if(renderMethod == OF_MESH_USING_VERTEX_ARRAY && vertexData->getNumIndices()){
			glDrawElements(mode, vertexData->getNumIndices(), GL_UNSIGNED_INT, vertexData->getIndexPointer());
		}
	}else{
		if(renderMethod == OF_MESH_USING_VBO){
			vbo.draw(mode,0,vertexData->getNumVertices());
		}else if(renderMethod == OF_MESH_USING_VERTEX_ARRAY){
			glDrawArrays(mode,0,vertexData->getNumVertices());
		}
	}
	
	if(renderMethod == OF_MESH_USING_VERTEX_ARRAY){
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
		
	glPopAttrib();
}

//--------------------------------------------------------------
void ofMesh::drawVertices(){
	draw(OF_MESH_POINTS);
}

//--------------------------------------------------------------
void ofMesh::drawWireframe(){
	draw(OF_MESH_WIREFRAME);
}

//--------------------------------------------------------------
void ofMesh::drawFaces(){
	draw(OF_MESH_FILL);
}
