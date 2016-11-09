#pragma once

#include "ofMesh.h"
#include "ofVbo.h"

class ofVboMesh: public ofMesh{
public:
	using ofMesh::draw;
	ofVboMesh();
	ofVboMesh(const ofMesh & mom);
    void operator=(const ofMesh & mom);
	virtual ~ofVboMesh();
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

	void draw(ofPolyRenderMode drawMode) const;
	void drawInstanced(ofPolyRenderMode drawMode, int primCount) const;
	
	ofVbo & getVbo();
	const ofVbo & getVbo() const;
	
private:
	void updateVbo();
	void unloadVbo();
	ofVbo vbo;
	int usage;
	std::size_t vboNumVerts;
	std::size_t vboNumIndices;
	std::size_t vboNumNormals;
	std::size_t vboNumTexCoords;
	std::size_t vboNumColors;
};
