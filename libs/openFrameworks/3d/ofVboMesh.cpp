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
		for (int j=0; j< mesh->faceType; j++){
			indices.push_back(mesh->faces[i].indices[j]);
		}
	}
	vbo.setIndexData(&indices[0], indices.size(),mesh->faceType );
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
	vbo.draw(GL_TRIANGLES,0,mesh->vertices.size());
}

//--------------------------------------------------------------
void ofVboMesh::drawFaces(){
	vbo.draw(GL_POLYGON,0,mesh->vertices.size());
}

//--------------------------------------------------------------
void ofVboMesh::addTriangles(const vector<ofVec3f>& verts){
	/*
	if(verts.size()%3==0){
	}else {
		//ofLog
		return;
	}
	 */
}

//--------------------------------------------------------------
void ofVboMesh::addTriangleFan(const vector<ofVec3f>& verts){
	//>3
}

//--------------------------------------------------------------
void ofVboMesh::addTriangleStrip(const vector<ofVec3f>& verts){
	//>3
}
