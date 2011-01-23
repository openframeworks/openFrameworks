#include "ofVboMesh.h"

//--------------------------------------------------------------
ofVboMesh::ofVboMesh(){
	vbo = ofVbo();
	meshElement = NULL;
	drawType = GL_STATIC_DRAW_ARB;
	bUseIndices = true;
}

//--------------------------------------------------------------
ofVboMesh::~ofVboMesh(){
}

//--------------------------------------------------------------
void ofVboMesh::setUseIndices(bool useIndices){
	bUseIndices = useIndices;
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
		//copy vec3 info
		meshElement->addVertex(verts[i]);
	}
}

//--------------------------------------------------------------
void ofVboMesh::update(){	
	if(meshElement->haveVertsChanged()){
		setupVertices(drawType);
		//vbo.updateVertexData(meshElement->getVerticesPointer(), meshElement->getNumVertices());
	}
	
	if(meshElement->haveColorsChanged()){
		setupColors(drawType);
		//vbo.updateColorData(meshElement->getColorsPointer(), meshElement->getNumColors());
	}
	
	if(meshElement->haveNormalsChanged()){
		setupNormals(drawType);
		//vbo.updateNormalData(meshElement->getNormalsPointer(), meshElement->getNumNormals());
	}
	
	if(meshElement->haveTexCoordsChanged()){
		setupTexCoords(drawType);
		//vbo.updateTexCoordData(meshElement->getTexCoordsPointer(), meshElement->getNumTexCoords());
	}	
	
	if(meshElement->haveIndicesChanged() && bUseIndices){
		setupIndices(drawType);
		//vbo.setIndexData(meshElement->getIndexPointer(),meshElement->getNumIndices());
		//vbo.updateIndexData(meshElement->getIndexPointer(),meshElement->getNumIndices());
	}
}

//--------------------------------------------------------------
void ofVboMesh::draw(polyMode pMode){
	if(!vbo.getIsAllocated()){
		setupVertices(drawType);
	}

	if(meshElement->getNumColors() && !vbo.getUsingColors()){
		setupColors(drawType);
	}
	
	if(meshElement->getNumNormals() && !vbo.getUsingNormals()){
		setupNormals(drawType);
	}
	
	if(meshElement->getNumTexCoords() && !vbo.getUsingTexCoords()){
		setupTexCoords(drawType);
	}
	
	if(!meshElement->getNumIndices()){
		meshElement->setupIndices();
	}
	
	update();
	
	glPushAttrib(GL_POLYGON_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, pMode);
	
	GLuint mode = ofGetGLTriangleMode(meshElement->getMode());
	
	if(pMode!=OF_MESH_POINTS){
		if(bUseIndices){
			cout << meshElement->getNumIndices() << endl;
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
