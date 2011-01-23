//Todd Vanderlin, Keith Pasko

//TODO: LOADS AND LOADS of GL error checking
//TODO: Add GLuint for vbo itself. make sure data is only sent to GPU if its updated, not otherwise
//TODO: Add edge flags

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
	GLuint indexId;
	
	GLuint vertId;
	GLuint colorId;
	GLuint normalId;
	GLuint texCoordId;
	
	bool bAllocated;
	
	bool bUsingVerts;		// need at least vertex data
	bool bUsingTexCoords;
	bool bUsingColors;
	bool bUsingNormals;
	bool bUsingIndices;
	
	int vertUsage;
	int colorUsage;
	int normUsage;
	int texUsage;
	
	float* vertData;
	float* normalData;
	float* texCoordData;
	float* colorData;
	GLuint* indexData;
	
public:
	
	ofVbo();
	~ofVbo();
	
	// you pass in a array of verts and the total amount of verts in that
	// array. The usage param is what you are going to do with the data.
	// OF_VBO_STATIC means that you are not going to manipulate the data
	// if you want to move to verts around you use OF_VBO_STREAM
	
	void setVertexData(const ofVec3f * verts, int total, int usage);
	void setColorData(const ofColor * colors, int total, int usage);	
	void setNormalData(const ofVec3f * normals, int total, int usage);	
	void setTexCoordData(const ofVec2f * texCoords, int total, int usage);	
	void setIndexData(const GLuint * indices, int total);
	
	//TODO: update methods
	
	float* getVertPointer();
	float* getColorPointer();
	float* getNormalPointer();
	float* getTexCoordPointer();
	GLuint* getIndexPointer();
	
	GLuint getVertId();
	GLuint getColorId();
	GLuint getNormalId();
	GLuint getTexCoordId();
	GLuint getIndexId();
	
	bool getIsAllocated();
	bool getUsingVerts();
	bool getUsingColors();
	bool getUsingNormals();
	bool getUsingTexCoords();
	bool getUsingIndices();
	
	void draw(int drawMode, int first, int total);
	void drawElements(int drawMode, int amt);
	void bind();
	void unbind();
	void clear();
};
