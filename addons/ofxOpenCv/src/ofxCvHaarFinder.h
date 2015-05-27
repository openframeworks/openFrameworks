#pragma once

/*
Originally written by charli_e, adapted from code by stefanix.
Completely rewritten by kyle mcdonald. opencv hack by theo watson.

See the discussion for more info:
http://www.openframeworks.cc/forum/viewtopic.php?f=10&t=2006

For a good documentation discussing the OpenCv haar finder:
http://www.comp.leeds.ac.uk/vision/opencv/opencvref_cv.html#decl_cvHaarDetectObjects
*/

#include "ofMain.h"
#include "ofxOpenCv.h"

// This hack was needed for older versions of OpenCv:
//#define HAAR_HACK

class ofxCvHaarFinder {
public:
	vector<ofxCvBlob> blobs;

	ofxCvHaarFinder();
	ofxCvHaarFinder(const ofxCvHaarFinder& finder);
	~ofxCvHaarFinder();
	
	void setup(string haarFile);
	
	// The default value is 1.2. For accuracy, bring it closer but not equal to 1.0. To make it faster, use a larger value.
	void setScaleHaar(float scaleHaar);
	// How many neighbors can be grouped into a face? Default value is 2. If set to 0, no grouping will be done.
	void setNeighbors(unsigned neighbors);

	int findHaarObjects(ofImage& input, int minWidth = 0, int minHeight = 0);
	int findHaarObjects(const ofxCvGrayscaleImage& input, int minWidth = 0, int minHeight = 0);
		
	int findHaarObjects(const ofxCvGrayscaleImage& input, ofRectangle& roi,	int minWidth = 0, int minHeight = 0);
	int findHaarObjects(const ofxCvGrayscaleImage&, int x, int y, int w, int h,	int minWidth = 0, int minHeight = 0);

	int findHaarObjects(ofPixels& input, int minWidth = 0, int minHeight = 0);

	float getWidth();
	float getHeight();
	
	void draw(float x, float y);

protected:
	CvHaarClassifierCascade* cascade;
	string haarFile;
	ofxCvGrayscaleImage img;
	float scaleHaar;
	unsigned neighbors;
};
