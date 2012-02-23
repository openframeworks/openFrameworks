#include "ofApp.h"

/*
 This code demonstrates the difference between using an ofMesh and an ofVboMesh.
 The ofMesh is uploaded to the GPU once per frame, while the ofVboMesh is
 uploaded once. This makes it much faster to draw multiple copies of an
 ofVboMesh than multiple copies of an ofMesh.
*/

/*
 These functions are for adding quads and triangles to an ofMesh -- either
 vertices, or texture coordinates.
*/
void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	mesh.addVertex(a);
	mesh.addVertex(b);
	mesh.addVertex(c);
}

void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

void addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c) {
	mesh.addTexCoord(a);
	mesh.addTexCoord(b);
	mesh.addTexCoord(c);
}

void addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c, ofVec2f d) {
	addTexCoords(mesh, a, b, c);
	addTexCoords(mesh, a, c, d);
}

/*
 The 3d data is stored in an image where alpha represents depth. Here we create
 a 3d point from the current x,y image position.
*/
ofVec3f getVertexFromImg(ofImage& img, int x, int y) {
	ofColor color = img.getColor(x, y);
	if(color.a > 0) {
		float z = ofMap(color.a, 0, 255, -480, 480);
		return ofVec3f(x - img.getWidth() / 2, y - img.getHeight() / 2, z);
	} else {
		return ofVec3f(0, 0, 0);
	}
}

void ofApp::setup() {
	img.loadImage("linzer.png");
	
	// OF_PRIMITIVE_TRIANGLES means every three vertices create a triangle
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	int skip = 1;	// this controls the resolution of the mesh
	int width = img.getWidth();
	int height = img.getHeight();
	ofVec3f zero(0, 0, 0);
	for(int y = 0; y < height - skip; y += skip) {
		for(int x = 0; x < width - skip; x += skip) {
			/*
			 To construct a mesh, we have to build a collection of quads made up of
			 the current pixel, the one to the right, to the bottom right, and
			 beneath. These are called nw, ne, se and sw. To get the texture coords
			 we need to use the actual image indices.
			*/
			ofVec3f nw = getVertexFromImg(img, x, y);
			ofVec3f ne = getVertexFromImg(img, x + skip, y);
			ofVec3f sw = getVertexFromImg(img, x, y + skip);
			ofVec3f se = getVertexFromImg(img, x + skip, y + skip);
			ofVec2f nwi(x, y);
			ofVec2f nei(x + skip, y);
			ofVec2f swi(x, y + skip);
			ofVec2f sei(x + skip, y + skip);
			
			// ignore any zero-data (where there is no depth info)
			if(nw != zero && ne != zero && sw != zero && se != zero) {
				addFace(mesh, nw, ne, se, sw);
				addTexCoords(mesh, nwi, nei, sei, swi);
			}
		}
	}
	
	vboMesh = mesh;
	
	glEnable(GL_DEPTH_TEST);
}

void ofApp::update() {
	
}

void ofApp::draw() {
	ofBackground(0);
	cam.begin();
	
	ofRotateY(ofGetElapsedTimef() * 30); // slowly rotate the model
	
	ofScale(1, -1, 1); // make y point down
	ofScale(.5, .5, .5); // make everything a bit smaller
	
	img.bind(); // bind the image to begin texture mapping
	int n = 5; // make a 5x5 grid
	ofVec2f spacing(img.getWidth(), img.getHeight()); // spacing between meshes
	ofTranslate(-spacing.x * n / 2, -spacing.y * n / 2, 0); // center the grid
	for(int i = 0; i < n; i++) { // loop through the rows
		for(int j = 0; j < n; j++) { // loop through the columns
			ofPushMatrix();
			ofTranslate(i * spacing.x, j * spacing.y); // position the current mesh
			ofTranslate(spacing.x / 2, spacing.y / 2); // center the mesh
			if(ofGetKeyPressed()) {
				vboMesh.draw(); // draw a vboMesh (faster) when a key is pressed
			} else {
				mesh.draw(); // draw an ofMesh (slower) when no key is pressed
			}
			ofPopMatrix();
		}
	}
	img.unbind();
	cam.end();
	
	// draw the framerate in the top left corner
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}