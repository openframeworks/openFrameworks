/*
 *  ofMeshElement.h
 *  openFrameworksLib
 *
 *  Created by Keith Pasko on 11/01/15.
 *  Copyright 2011 UCLA. All rights reserved.
 *
 */

// TODO:
// add/remove/edit vertices, 
// get face info

class ofMeshElement {
public:
	
protected:
	vector<ofVec3f> vertices;
	vector<ofVec3f> normals;
	vector<ofVec2f> texCoords;
	vector<ofColor> colors;
	vector<int> indicesSolid;
	vector<int> indicesWire;
	int mode;	// GL_TRIANGLES or GL_TRIANGLE_STRIP
	//ofMaterial *mat;
}
