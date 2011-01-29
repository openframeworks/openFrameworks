#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofShape.h"
#include "ofPath.h"

typedef struct {
	
	float 	x;
	float 	y;
	bool 	bBeingDragged;
	bool 	bOver;
	float 	radius;
	
}	draggableVertex;


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

	
	
	
	
	int nCurveVertexes;
	draggableVertex curveVertices[7];
	draggableVertex bezierVertices[4];
	
	
	ofPath pathA,pathB,pathC,pathD,pathE;
	ofPath pathF,pathFNonCurve;
	ofPath pathG,pathH;
	ofPath pathIa,pathIb,pathIc;
	ofPath selectedDraggableVertex, unselectedDraggableVertex;
	
};

#endif
