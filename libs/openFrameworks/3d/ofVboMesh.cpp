#include "ofVboMesh.h"

//--------------------------------------------------------------
ofVboMesh::ofVboMesh(){
	vbo = ofVbo();
	meshElement = NULL;
	drawType = GL_STATIC_DRAW_ARB;
	bEnableIndices = true;
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
	meshElement = v.getMeshElement();
	vbo = v.vbo;
}
 */

//--------------------------------------------------------------
void ofVboMesh::setMeshElement(ofMeshElement* m){
	meshElement = m;
}

//--------------------------------------------------------------
ofMeshElement* ofVboMesh::getMeshElement(){
	return meshElement;
}

//--------------------------------------------------------------
const ofMeshElement* ofVboMesh::getMeshElement() const{
	return meshElement;
}

//--------------------------------------------------------------
bool ofVboMesh::setupVertices(int usage){
	int size = meshElement->getNumVertices();
	if(size){
		vbo.setVertexData(meshElement->getVerticesPointer(),size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupVertices - no vertices in mesh.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofVboMesh::setupColors(int usage){
	int size = meshElement->getNumColors();
	if(size){
		vbo.setColorData(meshElement->getColorsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupColors - no colors in meshElement.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofVboMesh::setupNormals(int usage){
	int size = meshElement->getNumNormals();
	if(size){
		vbo.setNormalData(meshElement->getNormalsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupNormals - no normals in meshElement.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofVboMesh::setupTexCoords(int usage){
	int size = meshElement->getNumTexCoords();
	if(size){
		vbo.setTexCoordData(meshElement->getTexCoordsPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupTexCoords - no texCoords in meshElement.");
		return false;
	}
}

//--------------------------------------------------------------
bool ofVboMesh::setupIndices(int usage){
	int size = meshElement->getNumIndices();
	if(size){
		vbo.setIndexData(meshElement->getIndexPointer(), size, usage);
		return true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupIndices - no indices in meshElement.");
		return false;
	}
}

//--------------------------------------------------------------
void ofVboMesh::addMeshVertices(const vector<ofVec3f>& verts){
	for (int i =0; i < verts.size(); i++){
		meshElement->addVertex(verts[i]);
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
	if(meshElement->haveVertsChanged()){
		cout << "change" << endl;
		setupVertices(drawType);
		//vbo.updateVertexData(meshElement->getVerticesPointer(), meshElement->getNumVertices());
	}
	
	if(bEnableColors){
		if(meshElement->haveColorsChanged()){
			setupColors(drawType);
			//vbo.updateColorData(meshElement->getColorsPointer(), meshElement->getNumColors());
		}
	}

	if(bEnableNormals){
		if(meshElement->haveNormalsChanged()){
			setupNormals(drawType);
			//vbo.updateNormalData(meshElement->getNormalsPointer(), meshElement->getNumNormals());
		}
	}
	
	if(bEnableTexCoords){
		if(meshElement->haveTexCoordsChanged()){
			setupTexCoords(drawType);
			//vbo.updateTexCoordData(meshElement->getTexCoordsPointer(), meshElement->getNumTexCoords());
		}	
	}

	if(bEnableIndices){
		if(meshElement->haveIndicesChanged()){
			setupIndices(drawType);
		}
		//vbo.updateIndexData(meshElement->getIndexPointer(),meshElement->getNumIndices());
	}
}

//--------------------------------------------------------------
void ofVboMesh::draw(polyMode pMode){
	if(!vbo.getIsAllocated()){
		setupVertices(drawType);
	}
	
	if(bEnableColors){
		if(meshElement->getNumColors() && !vbo.getUsingColors()){
			setupColors(drawType);
		}
	}
	
	if(bEnableNormals){
		if(meshElement->getNumNormals() && !vbo.getUsingNormals()){
			setupNormals(drawType);
		}
	}
	
	if(bEnableTexCoords){
		if(meshElement->getNumTexCoords() && !vbo.getUsingTexCoords()){
			setupTexCoords(drawType);
		}
	}
	
	if(bEnableIndices){
		if(meshElement->getNumIndices() && !vbo.getUsingIndices()){
			setupIndices(drawType);
		}
	}
	
	update();
	
	glPushAttrib(GL_POLYGON_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, pMode);
	
	GLuint mode = ofGetGLTriangleMode(meshElement->getMode());
	
	if(pMode!=OF_MESH_POINTS){
		if(bEnableIndices){
			vbo.drawElements(mode,meshElement->getNumIndices());
		}else{
			vbo.draw(mode,0,meshElement->getNumVertices());
		}
	}else{
		//no indices needed for just drawing verts as points
		vbo.draw(GL_POINTS,0,meshElement->getNumVertices());
	}
	
	glPopAttrib();
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
