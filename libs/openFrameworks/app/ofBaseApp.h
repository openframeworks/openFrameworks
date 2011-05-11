#pragma once

#include "ofPoint.h"
#include "ofEvents.h"
#include "ofBaseTypes.h"

class ofBaseApp : public ofBaseSoundInput, public ofBaseSoundOutput{

	public:
        ofBaseApp() {
            mouseX = mouseY = 0;
        }

		virtual ~ofBaseApp(){}

		virtual void setup(){}
		virtual void update(){}
		virtual void draw(){}
		virtual void exit(){}

		virtual void windowResized(int w, int h){}

		virtual void keyPressed( int key ){}
		virtual void keyReleased( int key ){}

		virtual void mouseMoved( int x, int y ){}
		virtual void mouseDragged( int x, int y, int button ){}
		virtual void mousePressed( int x, int y, int button ){}
		virtual void mouseReleased(){}
		virtual void mouseReleased(int x, int y, int button ){}
		
		virtual void dragEvent(ofDragInfo dragInfo) { }
		virtual void gotMessage(ofMessage msg){ }		

//		virtual void audioIn( float * input, int bufferSize, int nChannels ){  
//			printf("ofBaseSoundInput 2\n");
//			audioReceived(input, bufferSize, nChannels);
//		}
//
//		virtual void audioReceived( float * input, int bufferSize, int nChannels ){}
//
//		virtual void audioOut( float * output, int bufferSize, int nChannels ){
//			printf("ofBaseSoundOutput 2\n");
//			audioRequested(output, bufferSize, nChannels);
//		}
//		virtual void audioRequested( float * output, int bufferSize, int nChannels ){}
		
		int mouseX, mouseY;			// for processing heads
};



