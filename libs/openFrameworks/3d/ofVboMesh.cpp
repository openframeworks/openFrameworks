#include "ofVboMesh.h"

//--------------------------------------------------------------
ofVboMesh::ofVboMesh(){
	vbo = ofVbo();
	vertexData = NULL;
	drawType = GL_STATIC_DRAW;
	bEnableIndices = false;
	bEnableColors = false;
	bEnableTexCoords = false;
	bEnableNormals = false;
}

//--------------------------------------------------------------
ofVboMesh::~ofVboMesh(){
}

//--------------------------------------------------------------
void ofVboMesh::setUseIndices(bool useIndices){
	bEnableIndices = useIndices;
}

//--------------------------------------------------------------
void ofVboMesh::setDrawType(int drawType_){
	drawType = drawType_;
}

/*
//--------------------------------------------------------------
ofVboMesh::ofVboMesh(const ofVboMesh& v){
	clone(v);
}

//--------------------------------------------------------------
ofVboMesh& ofVboMesh::operator=(const ofVboMesh& v){
	clone(v);
	return *this;
}

//--------------------------------------------------------------
void ofVboMesh::clone(const ofVboMesh& v){
	vertexData = v.getvertexData();
	vbo = v.vbo;
}
 */

//--------------------------------------------------------------
void ofVboMesh::setVertexData(ofVertexData* m){
	vertexData = m;
}

//--------------------------------------------------------------
ofVertexData* ofVboMesh::getVertexData(){
	return vertexData;
}

//--------------------------------------------------------------
const ofVertexData* ofVboMesh::getVertexData() const{
	return vertexData;
}

//--------------------------------------------------------------
bool ofVboMesh::setupVertices(int usage){
	int size = vertexData->getNumVertices();
	if(size){
		vbo.setVertexData(vertexData->getVerticesPointer(),size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupVertices - no vertices in vertexData.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofVboMesh::setupColors(int usage){
	int size = vertexData->getNumColors();
	if(size){
		vbo.setColorData(vertexData->getColorsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupColors - no colors in vertexData.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofVboMesh::setupNormals(int usage){
	int size = vertexData->getNumNormals();
	if(size){
		vbo.setNormalData(vertexData->getNormalsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupNormals - no normals in vertexData.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofVboMesh::setupTexCoords(int usage){
	int size = vertexData->getNumTexCoords();
	if(size){
		vbo.setTexCoordData(vertexData->getTexCoordsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupTexCoords - no texCoords in vertexData.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofVboMesh::setupIndices(int usage){
	int size = vertexData->getNumIndices();
	if(size){
		vbo.setIndexData(vertexData->getIndexPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupIndices - no indices in vertexData.");
		return false;
	}
}

//--------------------------------------------------------------
void ofVboMesh::enableColors(){
	bEnableColors = true;
}

//--------------------------------------------------------------
void ofVboMesh::disableColors(){
	bEnableColors = false;
}

//--------------------------------------------------------------
void ofVboMesh::enableNormals(){
	bEnableNormals = true;
}

//--------------------------------------------------------------
void ofVboMesh::disableNormals(){
	bEnableNormals = false;
}

//--------------------------------------------------------------
void ofVboMesh::enableTexCoords(){
	bEnableTexCoords = true;
}

//--------------------------------------------------------------
void ofVboMesh::disableTexCoords(){
	bEnableTexCoords = false;
}

//--------------------------------------------------------------
void ofVboMesh::update(){	
	if(vertexData->haveVertsChanged()){
		setupVertices(drawType);
		//vbo.updateVertexData(vertexData->getVerticesPointer(), vertexData->getNumVertices());
	}
	
	if(bEnableColors){
		if(vertexData->haveColorsChanged()){
			setupColors(drawType);
			//vbo.updateColorData(vertexData->getColorsPointer(), vertexData->getNumColors());
		}
	}

	if(bEnableNormals){
		if(vertexData->haveNormalsChanged()){
			setupNormals(drawType);
			//vbo.updateNormalData(vertexData->getNormalsPointer(), vertexData->getNumNormals());
		}
	}
	
	if(bEnableTexCoords){
		if(vertexData->haveTexCoordsChanged()){
			setupTexCoords(drawType);
			//vbo.updateTexCoordData(vertexData->getTexCoordsPointer(), vertexData->getNumTexCoords());
		}	
	}

	if(bEnableIndices){
		if(vertexData->haveIndicesChanged()){
			setupIndices(drawType);
		}
		//vbo.updateIndexData(vertexData->getIndexPointer(),vertexData->getNumIndices());
	}
}

//--------------------------------------------------------------
void ofVboMesh::draw(polyMode renderType){
	if(!vbo.getIsAllocated()){
		setupVertices(drawType);
	}
	
	if(bEnableColors){
		if(vertexData->getNumColors() && !vbo.getUsingColors()){
			setupColors(drawType);
		}
	}
	
	if(bEnableNormals){
		if(vertexData->getNumNormals() && !vbo.getUsingNormals()){
			setupNormals(drawType);
		}
	}
	
	if(bEnableTexCoords){
		if(vertexData->getNumTexCoords() && !vbo.getUsingTexCoords()){
			setupTexCoords(drawType);
		}
	}
	
	if(bEnableIndices){
		if(vertexData->getNumIndices() && !vbo.getUsingIndices()){
			setupIndices(drawType);
		}
	}
	
	update();
	
#ifndef TARGET_OPENGLES
	glPushAttrib(GL_POLYGON_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, renderType);
	
	GLuint mode = ofGetGLPrimitiveMode(vertexData->getMode());
#else
	GLuint mode;
	if(renderType==OF_MESH_WIREFRAME){
		mode = GL_LINES;
	}else{
		mode = ofGetGLPrimitiveMode(vertexData->getMode());
	}
#endif
	
	if(renderType!=OF_MESH_POINTS){
		if(bEnableIndices){
			vbo.drawElements(mode,vertexData->getNumIndices());
		}else{
			vbo.draw(mode,0,vertexData->getNumVertices());
		}
	}else{
		//no indices needed for just drawing verts as points
		vbo.draw(GL_POINTS,0,vertexData->getNumVertices());
	}
	
#ifndef TARGET_OPENGLES
	glPopAttrib();
#endif
}

//--------------------------------------------------------------
void ofVboMesh::drawVertices(){
	draw(OF_MESH_POINTS);
}

//--------------------------------------------------------------
void ofVboMesh::drawWireframe(){
	draw(OF_MESH_WIREFRAME);
}

//--------------------------------------------------------------
void ofVboMesh::drawFaces(){
	draw(OF_MESH_FILL);
}
