/*
 * ofShapeGLRenderers.cpp
 *
 *  Created on: 21/01/2011
 *      Author: arturo
 */
#include "ofShapeGLRenderers.h"
#include "ofShape.h"
#include "ofMesh.h"
#include "ofPath.h"

void ofVboRenderer::draw(ofShape & shape){
	ofMesh & mesh = shape.getMesh();
	vbos.resize(mesh.elements.size());
	for(int i=0; i<(int)mesh.elements.size(); i++){
		vbos[i].setDrawType(GL_STATIC_DRAW_ARB);
		vbos[i].setUseIndices(false);
		vbos[i].setMeshElement(&mesh.elements[i]);
		if(shape.isFilled())
			vbos[i].drawFaces();
		else
			vbos[i].drawWireframe();
	}
	if(shape.hasOutline()){
		if(shape.getOutline().hasChanged()){
			vboOutline.setVertexData(&shape.getOutline().getVertices()[0],shape.getOutline().size(),GL_STATIC_DRAW_ARB);
		}
		vboOutline.draw(GL_LINE_LOOP,0,shape.getOutline().size());
	}
}

void ofVboRenderer::draw(ofPath & path){
	ofShape shape = path.getShape();
	draw(shape);
}

void ofVARenderer::draw(ofShape & shape){
	ofMesh & mesh = shape.getMesh();
	glEnableClientState(GL_VERTEX_ARRAY);
	for(int i=0;i<(int)mesh.elements.size();i++){

		ofTriangleMode mode = mesh.elements[i].getMode();
		GLuint currentTriType = ofGetGLTriangleMode(mode);
		glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &mesh.elements[i].getVerticesPointer()->x);
		glDrawArrays(currentTriType, 0, mesh.elements[i].getNumVertices());
	}
	if(shape.hasOutline()){
		glVertexPointer(shape.getOutline().is3D()?3:2, GL_FLOAT, sizeof(ofVec3f), &shape.getOutline().getVertices()[0].x);
		glDrawArrays(shape.getOutline().isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, shape.getOutline().size());
	}
}

void ofVARenderer::draw(ofPath & path){
	ofShape shape = path.getShape();
	draw(shape);
}
