#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
    bool bFill;
    bool bWireframe;
    bool bDrawNormals;
    bool bDrawAxes;
    bool bDrawLights;
    bool bInfoText;
    bool bMousePressed;
    bool bSplitFaces;
    
    ofImage texture;
    ofVideoGrabber vidGrabber;
    int mode;
    
    ofSpherePrimitive sphere;
    ofIcoSpherePrimitive icoSphere;
    ofPlanePrimitive plane;
    ofCylinderPrimitive cylinder;
    ofConePrimitive cone;
    ofBoxPrimitive box;
    
    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    ofMaterial material;
    
    // place to store the sides of the box //
    ofMesh boxSides[ofBoxPrimitive::SIDES_TOTAL];
    ofMesh deformPlane;
    vector<ofMeshFace> triangles;
    
};
