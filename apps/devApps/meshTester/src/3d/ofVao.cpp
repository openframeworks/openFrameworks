#include "ofVao.h"

ofVao::ofVao(){
}

ofVao::~ofVao(){
}

void ofVao::setIndexData(const int * indices, int total, int type){
	if(indices == NULL){
		ofLog(OF_LOG_WARNING,"ofVao: bad index data!\n");
		return;
	}
		
	glGenBuffers(1, &indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * total * type, &indices[0], GL_STATIC_DRAW);
}

void ofVao::bind(){
	glGenVertexArrays(1, &handle);
	glBindVertexArray(handle);
	
	/*
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT,sizeof(ofVec3f), vbo->getNormalPointer());
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(ofVec2f), vbo->getTexCoordPointer());
	//TODO: handle second texture/color
	*/
	
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, sizeof(ofColor), vbo->getColorPointer());
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), vbo->getVertPointer());
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	
	glBindVertexArray(0);
}