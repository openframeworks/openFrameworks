#include "ofVboMesh.h"

//--------------------------------------------------------------
ofVboMesh::ofVboMesh(){
	mesh = new ofMesh();
}

//--------------------------------------------------------------
ofVboMesh::~ofVboMesh(){
}

//--------------------------------------------------------------
void ofVboMesh::setMesh(ofMesh* m){
	delete mesh;
	mesh = m;
}

//--------------------------------------------------------------
ofMesh* ofVboMesh::getMesh(){
	return mesh;
}

void ofVboMesh::clear(){
	indices.clear();
	mesh->clear();
}

//--------------------------------------------------------------
void ofVboMesh::setupVertices(int usage){
	vbo.setVertexData(&mesh->vertices[0], mesh->vertices.size(), usage);
}

//--------------------------------------------------------------
void ofVboMesh::setupColors(int usage){
	vbo.setColorData(&mesh->colors[0], mesh->numColors(), usage);
	mesh->bUsingColors = true; 
}

//--------------------------------------------------------------
void ofVboMesh::setupIndices(){
	for (int i = 0; i < mesh->faces.size();i++){
		for (int j=0; j< mesh->faces[i].indices.size(); j++){
			indices.push_back((GLuint)mesh->faces[i].indices[j]);
		}
	}
	vbo.setIndexData(&indices[0],indices.size() );
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
void ofVboMesh::drawVertices(){
	vbo.draw(GL_POINTS,0,mesh->vertices.size());
}

//--------------------------------------------------------------
void ofVboMesh::drawWireframe(){
	ofLog(OF_LOG_ERROR,"ofVboMesh: wireframes not ready yet, sorry");
//	vbo.draw(GL_POINTS,0,mesh->vertices.size());
}

//--------------------------------------------------------------
void ofVboMesh::drawFaces(){
	vbo.draw(indices.size(),GL_TRIANGLES);
}

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
