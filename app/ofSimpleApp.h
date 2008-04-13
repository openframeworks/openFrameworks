#ifndef _OF_SIMPLE_APP
#define _OF_SIMPLE_APP

#include "ofConstants.h"
#include "ofUtils.h"

class ofSimpleApp{
	
	public:
	
		ofSimpleApp(){ 
			mouseX = 0; 
			mouseY = 0;
		}

		virtual ~ofSimpleApp(){}
		
		virtual void setup(){}
		virtual void update(){}
		virtual void draw(){}
		virtual void exit(){}
		
		
		virtual void keyPressed(int key ){}
		virtual void keyReleased(int key ){}
		
		virtual void mouseMoved( int x, int y ){}
		virtual void mouseDragged( int x, int y, int button ){}
		virtual void mousePressed( int x, int y, int button ){}
		virtual void mouseReleased(){}
	
		virtual void audioReceived 	(float * input, int bufferSize, int nChannels) {} 
		virtual void audioRequested (float * output, int bufferSize, int nChannels) {}	

		int mouseX, mouseY;			// for processing heads
};

#endif	


