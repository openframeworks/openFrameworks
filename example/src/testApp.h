#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#include "ofxVectorMath.h"

#define MAX_N_PTS         1500

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased (int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		ofxVec3f	pts[MAX_N_PTS];
		int			nPts;

		ofxVec3f	rotateAmount;  // amount to rotate in x,y,z;
		float		speedOfRotation; // speed;

		// a grid helpful for seeing the rotation
		ofxVec3f	xAxisMin;
		ofxVec3f	xAxisMax;
		ofxVec3f	yAxisMin;
		ofxVec3f	yAxisMax;
		ofxVec3f	zAxisMin;
		ofxVec3f	zAxisMax;

		bool		bDrawnAnything;

};

#endif

