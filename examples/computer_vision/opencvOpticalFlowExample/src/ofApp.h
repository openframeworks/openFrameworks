#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file

class SpinCube {
public:
	glm::vec2 pos = {0,0};
	float xAngleAcc = 0.0;
	float xAxisAngle = 0.0f;
	float yAxisAngle = 0.0f;
	float yAngleAcc = 0.0;
	float size = 30.0;
};

class Particle {
public:
	glm::vec2 pos = {0,0};
	glm::vec2 basePos = {0,0};
	glm::vec2 vel = {0,0};
	
	float size = 30.0;
	float timeNotTouched = 0.0f;
	bool bAtBasePos = false;
};

class FlowLine {
public:
	glm::vec2 pos = {0,0};
	glm::vec2 flow = {1,0};
	glm::vec2 target = {1,0};
	float targetAngle = 0.0;
	float angle = 0.0f;
	float brightness = 0.5;
	float size = 30.0;
};

class ofApp : public ofBaseApp{
	public:
		enum AppMode {
			MODE_SPIN_CUBES=0,
			MODE_PARTICLES,
			MODE_LINES
		};
	
		void setup();
		void update();
		void draw();
	
		glm::vec2 getOpticalFlowValueForPercent( float xpct, float ypct );
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif

        ofxCvColorImage			colorImg;
		ofxCvGrayscaleImage 	grayImage;
		
		ofxCvGrayscaleImage currentImage;
		cv::Mat previousMat;
		cv::Mat flowMat;
		
		vector<SpinCube> spinCubes;
		vector<Particle> particles;
		vector<FlowLine> flowLines;

		int blurAmount;
		bool bMirror;
		float cvDownScale;
		bool bContrastStretch;
		float minLengthSquared;
		bool bDrawOptiFlowVectors;
		int mode;
		
		ofVboMesh arrowMesh;
		ofVboMesh planeMesh;

};

