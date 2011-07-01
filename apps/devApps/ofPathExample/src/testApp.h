#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofPath.h"
#include "ofCairoRenderer.h"

// uncomment to render to pdf
//#define RENDER_TO_PDF

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
	
	
	ofPath shapeA,shapeB,shapeC,shapeD,shapeE;
	ofPath shapeF,shapeFNonCurve;
	ofPath shapeG,shapeH;
	ofPath shapeIa,shapeIb,shapeIc;
	ofPath selectedDraggableVertex, unselectedDraggableVertex;
};

#endif
