#include "ofMesh.h"
#include "ofGraphics.h"

//--------------------------------------------------------------
ofMesh::ofMesh(){
	vbo = ofVbo();
	vertexData = NULL;
	renderMethod = OF_MESH_USING_DEFAULT_RENDERER;
	drawType = GL_STATIC_DRAW_ARB;
	bEnableIndices = false;
	bEnableColors = false;
	bEnableTexCoords = false;
	bEnableNormals = false;
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
	if(!vbo.getIsAllocated()){
		setupVerticesForVbo(drawType);
	}
	
	if (vertexData->haveVertsChanged()){
		vbo.updateVertexData(vertexData->getVerticesPointer(), vertexData->getNumVertices());
	}
	
	if(bEnableColors){
		
		if(vertexData->getNumColors() && !vbo.getUsingColors()){
			setupColorsForVbo(drawType);
		}
		
		if(vertexData->haveColorsChanged()){
			vbo.updateColorData(vertexData->getColorsPointer(), vertexData->getNumColors());
		}
	}
	
	if(bEnableNormals){
		if(vertexData->getNumNormals() && !vbo.getUsingNormals()){
			setupNormalsForVbo(drawType);
		}
		
		if(vertexData->haveNormalsChanged()){
			vbo.updateNormalData(vertexData->getNormalsPointer(), vertexData->getNumNormals());
		}
	}
	
	if(bEnableTexCoords){
		if(vertexData->getNumTexCoords() && !vbo.getUsingTexCoords()){
			setupTexCoordsForVbo(drawType);
		}
		
		if(vertexData->haveTexCoordsChanged()){
			vbo.updateTexCoordData(vertexData->getTexCoordsPointer(), vertexData->getNumTexCoords());
		}
	}
	
	if(bEnableIndices){
		if(vertexData->getNumIndices() && !vbo.getUsingIndices()){
			setupIndicesForVbo(drawType);
		}
		
		if(vertexData->haveIndicesChanged()){
			vbo.updateIndexData(vertexData->getIndexPointer(), vertexData->getNumIndices());
		}
	}	
}

//--------------------------------------------------------------
void ofMesh::draw(polyMode renderType){
	if(renderMethod == OF_MESH_USING_VBO){
		setupVbo();
	}
	
	glPushAttrib(GL_POLYGON_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
	
	GLuint mode = ofGetGLPrimitiveMode(vertexData->getMode());
	
	if(renderMethod == OF_MESH_USING_VBO){
		if(bEnableIndices){
			vbo.drawElements(mode,vertexData->getNumIndices());
		}else{
			vbo.draw(mode,0,vertexData->getNumVertices());
		}
	}else{
		ofGetDefaultRenderer()->draw(*vertexData);
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
