#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#define OF_ADDON_USING_OFXDIRLIST

#include "ofAddons.h"


class testApp : public ofSimpleApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void keyReleased (int key);
		
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		// we will have a dynamic number of images, based on the content of a director:
              
		int 		nImages;
		ofImage	* 	images;
        ofxDirList   DIR;
        
        int         currentImage;
		
};

#endif
	
