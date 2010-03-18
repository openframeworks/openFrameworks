#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxDirList.h"

#include "ofxiPhoneExtras.h"

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		// we will have a dynaimic number of images, based on the content of a director:

		int 		nImages;
		ofImage	* 	images;
        ofxDirList   DIR;

        int         currentImage;

};

#endif

