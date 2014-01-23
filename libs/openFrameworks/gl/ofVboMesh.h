#pragma once

#include "ofMesh.h"
#include "ofVbo.h"

class ofVboMesh: public ofMesh{
public:
	using ofMesh::draw;
	ofVboMesh();
	ofVboMesh(const ofMesh & mom);
	void operator=(const ofMesh & mom);
	void setUsage(int usage);

    void enableColors();
    void enableTextures();
    void enableNormals();
    void enableIndices();

    void disableColors();
    void disableTextures();
    void disableNormals();
    void disableIndices();

    virtual bool usingColors() const;
    virtual bool usingTextures() const;
    virtual bool usingNormals() const;
    virtual bool usingIndices() const;

	void draw(ofPolyRenderMode drawMode);
	void drawInstanced(ofPolyRenderMode drawMode, int primCount);
	
	ofVbo & getVbo();
	
private:
	void updateVbo();
	ofVbo vbo;
	int usage;
	int vboNumVerts, vboNumIndices, vboNumNormals, vboNumTexCoords, vboNumColors;
};
