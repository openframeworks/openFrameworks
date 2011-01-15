#include "ofVboMesh.h"

//--------------------------------------------------------------
ofVboMesh::ofVboMesh(){
	vbo = ofVbo();
	mesh = NULL;
	bIsInited = false;
	mode = -1;
}

//--------------------------------------------------------------
ofVboMesh::~ofVboMesh(){
}

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
	mesh = v.getMesh();
	vbo = v.vbo;
	mode = v.mode;
	bIsInited = v.bIsInited;
}

//--------------------------------------------------------------
void ofVboMesh::setMesh(ofMesh* m){
	mesh = m;
}

//--------------------------------------------------------------
ofMesh* ofVboMesh::getMesh(){
	return mesh;
}

//--------------------------------------------------------------
ofMesh* ofVboMesh::getMesh() const{
	return mesh;
}

void ofVboMesh::clear(){
	indices.clear();
	mesh->clear();
}

//--------------------------------------------------------------
void ofVboMesh::setupVertices(int usage){
	if(mesh->vertices.size()){
		vbo.setVertexData(&(mesh->vertices[0]), mesh->vertices.size(), usage);
		bIsInited = true;
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupVertices - no vertices in mesh.");
	}
}

//--------------------------------------------------------------
void ofVboMesh::setupColors(int usage){
	if(mesh->colors.size()){
		vbo.setColorData(&mesh->colors[0], mesh->numColors(), usage);
		mesh->bUsingColors = true; 
	}else{
		ofLog(OF_LOG_WARNING,"ofVboMesh:setupVertices - no vertices in mesh.");
	}
}

//--------------------------------------------------------------
void ofVboMesh::setupIndices(int indexMode){
	switch(indexMode){
		case(OF_MESH_POINTS):
		case(OF_MESH_FILL):
			indices.clear();
			for (int i = 0; i < mesh->faces.size();i++){
				for (int j=0; j< mesh->faces.at(i).indices.size(); j++){
					indices.push_back((GLuint)mesh->faces.at(i).indices.at(j));
				}
			}
			vbo.setIndexData(&indices[0], indices.size() );
			break;
		case(OF_MESH_WIREFRAME):
			wfIndices.clear();
			for (int i=0;i<mesh->faces.size();i++){
				
				int maxIndex = mesh->faces.at(i).indices.size()-1;
				
				for (int j=0;j<maxIndex;j++){
					wfIndices.push_back( (GLuint) mesh->faces.at(i).indices.at(j) );
					wfIndices.push_back( (GLuint) mesh->faces.at(i).indices.at(j+1) );
				}
				
				wfIndices.push_back((GLuint)mesh->faces.at(i).indices.at(maxIndex));
				wfIndices.push_back((GLuint)mesh->faces.at(i).indices.at(0));
			}
			break;
		default:break;
	}
}

/*
 void setupNormals(int usage){
 vbo->setNormalData(&mesh->normals[0], mesh->numNormals(), usage);
 }
 void setupTextures(int usage){
 vbo->setTexCoordData(&mesh->texCoords[0], mesh->numTexCoords(), usage);
 }
 */

//--------------------------------------------------------------
void ofVboMesh::addTriangles(const vector<ofVec3f>& verts){
	if(verts.size()%3==0){
		//store current size of the mesh vertices
		int curSize = mesh->vertices.size();
		addMeshVertices(verts);
		for (int i = 0; i < verts.size()-2;i++){
			int index = i+curSize;
			mesh->addFace(index, index+1, index+2);
			indices.push_back(index);
			indices.push_back(index+1);
			indices.push_back(index+2);
		}
	}else {
		ofLog(OF_LOG_ERROR,"ofVboMesh::addTriangles: trying to create triangles from non-divisble-by-three array");
		return;
	}
}

//--------------------------------------------------------------
void ofVboMesh::addTriangleFan(const vector<ofVec3f>& verts){
	if(verts.size()>=3){
		//store current size of the mesh vertices
		int curSize = mesh->vertices.size();
		addMeshVertices(verts);
		for (int i = 1; i < verts.size()-1;i++){
			int index = i+curSize;
			mesh->addFace(0, index, index+1);
			indices.push_back(0);
			indices.push_back(index);
			indices.push_back(index+1);
		}
	}else {
		ofLog(OF_LOG_ERROR,"ofVboMesh::addTriangleFan: must supply at least 3 vertices");
		return;
	}
}

//--------------------------------------------------------------
void ofVboMesh::addTriangleStrip(const vector<ofVec3f>& verts){
	if(verts.size()>=3){
		//store current size of the mesh vertices
		int curSize = mesh->vertices.size();
		addMeshVertices(verts);
		for (int i = 0; i < verts.size()-1;i++){
			int index = i+curSize;
			mesh->addFace(index, index+1, index+2);
		}
	}else {
		ofLog(OF_LOG_ERROR,"ofVboMesh::addTriangleStrip: must supply at least 3 vertices");
		return;
	}
}

//--------------------------------------------------------------
void ofVboMesh::addMeshVertices(const vector<ofVec3f>& verts){
	for (int i =0; i < verts.size(); i++){
		//copy vec3 info
		mesh->vertices.push_back(verts[i]);
		//if we want to use this info, we need to match the vert size
		if(mesh->bUsingTexCoords) mesh->texCoords.push_back(ofVec2f(0,0));
		if(mesh->bUsingColors) mesh->colors.push_back(ofColor(255,255,255,255));
	}
}

//--------------------------------------------------------------
void ofVboMesh::drawVertices(){
	//make sure we have vertices
	if(!bIsInited){
		setupVertices(OF_VBO_STATIC);
	}
	
	//make sure we have indices
	if(!indices.size()){
		setupIndices(OF_MESH_POINTS);
	}
	
	//feed vbo the correct index data
	if(mode == OF_MESH_WIREFRAME){
		vbo.setIndexData(&indices[0], indices.size() );
	}

	vbo.draw(GL_POINTS,0,mesh->vertices.size());
	mode = OF_MESH_POINTS;
}

//--------------------------------------------------------------
void ofVboMesh::drawWireframe(){
	//make sure we have vertices
	if(!bIsInited){
		setupVertices(OF_VBO_STATIC);
	}
	
	//make sure we have indices
	if(!wfIndices.size()){
		setupIndices( OF_MESH_WIREFRAME );
	}
	
	//tell vbo the correct indexing data
	if(mode != OF_MESH_WIREFRAME ){
		vbo.setIndexData(&wfIndices[0], wfIndices.size() );
	}
	
	vbo.draw(wfIndices.size(),GL_LINES);
	mode = OF_MESH_WIREFRAME;
}

//--------------------------------------------------------------
void ofVboMesh::drawFaces(){
	//make sure we have vertices
	if(!bIsInited){
		setupVertices(OF_VBO_STATIC);
	}
	
	//make sure we have indices
	if(!indices.size()){
		setupIndices(OF_MESH_FILL);
	}
	
	//tell vbo the correct indexing data
	if (mode == OF_MESH_WIREFRAME){
		vbo.setIndexData(&indices[0], indices.size() );
	}
	
	vbo.draw(indices.size(),GL_TRIANGLES);
	mode = OF_MESH_FILL;
}
