#ifndef _OF_BASE_APP
#define _OF_BASE_APP

#include "ofTypes.h"
#include "ofEvents.h"

class ofBaseApp{

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

		virtual void audioReceived( float * input, int bufferSize, int nChannels ){}
		virtual void audioRequested( float * output, int bufferSize, int nChannels ){}

		virtual void touchDown(float x, float y, int touchId){}
		virtual void touchMoved(float x, float y, int touchId){}
		virtual void touchUp(float x, float y, int touchId){}

		virtual void touchDown(ofTouchEventArgs & touch){
			touchDown(touch.x, touch.y, touch.id);
		}

		virtual void touchMoved(ofTouchEventArgs & touch){
			touchMoved(touch.x, touch.y, touch.id);
		}

		virtual void touchUp(ofTouchEventArgs & touch){
			touchUp(touch.x,touch.y,touch.id);
		}


		int mouseX, mouseY;			// for processing heads
};

#endif


