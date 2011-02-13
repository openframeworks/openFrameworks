//TODO: should textures be ofTexture or ofImage (dynamically drawing to model texture?)

#pragma once

#include "ofMesh.h"
#include "ofUtils.h"
#include "ofImage.h"
#include "ofConstants.h"
#include <map>

typedef pair<string, ofVertexData*> ofNamedVertexData;
typedef map<string, ofVertexData*> ofNamedVerticesData;
typedef pair<string, ofMesh*> ofNamedMesh;
typedef map<string, ofMesh*> ofNamedMeshes;

class ofModel{
public:
	ofModel();
	~ofModel();
		
	void drawWireframe();
	void drawFaces();
	void drawVertices();
	
	void enableTextures();
	void disableTextures();
	void enableNormals();
	void disableNormals();
	void enableColors();
	void disableColors();
	
	void bindTextureForMesh(int id);
	void unbindTextureForMesh(int id);
	
	void setRenderMethod(meshRenderMethod m);
	

	ofMesh* getMesh(unsigned int nIndex);
	ofMesh* getMesh(string sName);
	void listMeshNames();

	vector<ofMesh> meshes;
	vector<ofImage> textures;
	map<int, int> textureLinks;
	

	ofNamedMeshes named_meshes;
	ofNamedVerticesData named_vertices;
	
protected:
	bool bUsingTextures;

private:
	meshRenderMethod renderMethod;
};
