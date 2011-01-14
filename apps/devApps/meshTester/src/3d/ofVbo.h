/*
 *  ofVbo.h
 *  ofVBO
 *
 *  Created by Todd Vanderlin on 1/12/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

enum {
	OF_VBO_VERTEX,
	OF_VBO_NORMAL,
	OF_VBO_COLOR,
	OF_VBO_TEX_COORDS
};

enum {
	OF_VBO_STATIC = GL_STATIC_DRAW,	
	OF_VBO_STREAM = GL_STREAM_DRAW
};


class ofVbo {
	
private:
	
	GLuint vertId;
	GLuint colorId;
	GLuint normalId;
	GLuint texCoordId;
	
	GLuint handle;
	
	bool bAllocated;
	bool bUsingVerts;		// need at least vertex data
	bool bUsingTexCoords;
	bool bUsingColors;
	bool bUsingNormals;
	
	int vertUsage;
	int colorUsage;
	int normUsage;
	int texUsage;
	
	float* vertData;
	float* normalData;
	float* texCoordData;
	float* colorData;
	
public:
	
	ofVbo();
	
	//extern void glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
	//glBufferData(GL_ARRAY_BUFFER_ARB, NUM_PARTICLES*4*sizeof(float), color, GL_STREAM_DRAW_ARB);
	
	// you pass in a array of verts and the total amount of verts in that
	// array. The usage param is what you are going to do with the data.
	// OF_VBO_STATIC means that you are not going to manipulate the data
	// if you want to move to verts around you use OF_VBO_STREAM
	void setVertexData(const ofVec3f * verts, int total, int usage);
	void setColorData(const ofColor * colors, int total, int usage);	
	
	void updateColorData(const ofColor * colors, int total);
	void updateVertexData(const ofVec3f * verts, int total);	
	
	//void setNormalData() {}
	//void setTexCoordData() {}
	
	float* getVertPointer();
	float* getColorPointer();
		/*
	float* getNormalPointer();
	float* getTexCoordPointer();
	 */
	
	GLuint getVertId();
	GLuint getColorId();
	GLuint getNormalId();
	GLuint getTexCoordId();
	
	
	
	void draw(int mode, int first, int total);
	void bind();
	void unbind();
};
