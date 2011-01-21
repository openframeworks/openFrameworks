/*
 * ofShapeGLRenderers.cpp
 *
 *  Created on: 21/01/2011
 *      Author: arturo
 */
#include "ofShapeGLRenderers.h"
#include "ofShape.h"
#include "ofMesh.h"

//---------------------------------------------------
void ofVboShapeRenderer::setShape(ofShape & shape_){
	shape=&shape_;
}
void ofVboShapeRenderer::draw(){
	ofMesh & mesh = shape->getMesh();
	vbos.resize(mesh.elements.size());
	for(int i=0; i<mesh.elements.size(); i++){
		vbos[i].setMeshElement(&mesh.elements[i]);
		vbos[i].drawFaces();
	}
}


//---------------------------------------------------
void ofVAShapeRenderer::setShape(ofShape & shape_){
	shape=&shape_;
}

void ofVAShapeRenderer::draw(){
	ofMesh & mesh = shape->getMesh();
	glEnableClientState(GL_VERTEX_ARRAY);
	for(int i=0;i<mesh.elements.size();i++){

		ofTriangleType mode = mesh.elements[i].getMode();
		GLuint currentTriType = GL_LINE_LOOP;
		switch(mode){
		case OF_TRIANGLES_ELEMENT:
			currentTriType=GL_TRIANGLES;
			break;
		case OF_TRIANGLE_STRIP_ELEMENT:
			currentTriType=GL_TRIANGLE_STRIP;
			break;
		case OF_TRIANGLE_FAN_ELEMENT:
			currentTriType=GL_TRIANGLE_FAN;
			break;
		}
		glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &mesh.elements[i].getVerticesPointer()->x);
		glDrawArrays(currentTriType, 0, mesh.elements[i].getNumVertices());
	}
}
