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
		vbos[i].setDrawType(GL_STATIC_DRAW_ARB);
		vbos[i].setUseIndices(false);
		vbos[i].setMeshElement(&mesh.elements[i]);
		if(shape->isFilled())
			vbos[i].drawFaces();
		else
			vbos[i].drawWireframe();
	}
	if(shape->hasOutline()){
		if(shape->getPolyline().hasChanged()){
			vboOutline.setVertexData(&shape->getPolyline().getVertices()[0],shape->getPolyline().size(),GL_STATIC_DRAW_ARB);
		}
		vboOutline.draw(GL_LINE_LOOP,0,shape->getPolyline().size());
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

		ofTriangleMode mode = mesh.elements[i].getMode();
		GLuint currentTriType = ofGetGLTriangleMode(mode);
		glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &mesh.elements[i].getVerticesPointer()->x);
		glDrawArrays(currentTriType, 0, mesh.elements[i].getNumVertices());
	}
	if(shape->hasOutline()){
		glVertexPointer(shape->getPolyline().is3D()?3:2, GL_FLOAT, sizeof(ofVec3f), &shape->getPolyline().getVertices()[0].x);
		glDrawArrays(shape->getPolyline().isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, shape->getPolyline().size());
	}
}
