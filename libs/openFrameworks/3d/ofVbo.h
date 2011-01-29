//TODO: Speedtest repeated setup vs. update call
//TODO: Add edge flags?

#pragma once
#include "ofMain.h"

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
	GLubyte* indexData;
	
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
	void setIndexData(const GLubyte * indices, int total, int usage);
	
	/*
	void updateVertexData(const ofVec3f * verts, int total);
	void updateColorData(const ofColor * colors, int total);	
	void updateNormalData(const ofVec3f * normals, int total);	
	void updateTexCoordData(const ofVec2f * texCoords, int total);	
	void updateIndexData(const GLuint * indices, int total);
	 */
	
	float* getVertPointer();
	float* getColorPointer();
	float* getNormalPointer();
	float* getTexCoordPointer();
	GLubyte* getIndexPointer();
	
	GLuint getVertId();
	GLuint getColorId();
	GLuint getNormalId();
	GLuint getTexCoordId();
	GLubyte getIndexId();
	
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
