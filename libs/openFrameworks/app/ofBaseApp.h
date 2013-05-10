#pragma once

#include "ofPoint.h"
#include "ofEvents.h"
#include "ofBaseTypes.h"

class ofBaseApp : public ofBaseSoundInput, public ofBaseSoundOutput{

	public:
        ofBaseApp() {
            mouseX = mouseY = 0;
            ofAddListener(ofEvents().setup,this,&ofBaseApp::setup,OF_EVENT_ORDER_APP);
            ofAddListener(ofEvents().update,this,&ofBaseApp::update,OF_EVENT_ORDER_APP);
            ofAddListener(ofEvents().draw,this,&ofBaseApp::draw,OF_EVENT_ORDER_APP);
            ofAddListener(ofEvents().exit,this,&ofBaseApp::exit,OF_EVENT_ORDER_APP);
            ofAddListener(ofEvents().windowResized,this,&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
            ofAddListener(ofEvents().keyPressed,this,&ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
            ofAddListener(ofEvents().keyReleased,this,&ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
            ofAddListener(ofEvents().mouseMoved,this,&ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
            ofAddListener(ofEvents().mouseDragged,this,&ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
            ofAddListener(ofEvents().mousePressed,this,&ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
            ofAddListener(ofEvents().mouseReleased,this,&ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
        }

		virtual ~ofBaseApp(){
            ofRemoveListener(ofEvents().setup,this,&ofBaseApp::setup,OF_EVENT_ORDER_APP);
            ofRemoveListener(ofEvents().update,this,&ofBaseApp::update,OF_EVENT_ORDER_APP);
            ofRemoveListener(ofEvents().draw,this,&ofBaseApp::draw,OF_EVENT_ORDER_APP);
            ofRemoveListener(ofEvents().exit,this,&ofBaseApp::exit,OF_EVENT_ORDER_APP);
            ofRemoveListener(ofEvents().windowResized,this,&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
            ofRemoveListener(ofEvents().keyPressed,this,&ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
            ofRemoveListener(ofEvents().keyReleased,this,&ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
            ofRemoveListener(ofEvents().mouseMoved,this,&ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
            ofRemoveListener(ofEvents().mouseDragged,this,&ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
            ofRemoveListener(ofEvents().mousePressed,this,&ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
            ofRemoveListener(ofEvents().mouseReleased,this,&ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
		}

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
	
		virtual void windowEntry ( int state ) { }
		
		int mouseX, mouseY;			// for processing heads

	protected:
		virtual void setup(ofEventArgs & args){
			setup();
		}
		virtual void update(ofEventArgs & args){
			update();
		}
		virtual void draw(ofEventArgs & args){
			draw();
		}
		virtual void exit(ofEventArgs & args){
			exit();
		}

		virtual void windowResized(ofResizeEventArgs & resize){
			windowResized(resize.width,resize.height);
		}

		virtual void keyPressed( ofKeyEventArgs & key ){
			keyPressed(key.key);
		}
		virtual void keyReleased( ofKeyEventArgs & key ){
			keyPressed(key.key);
		}

		virtual void mouseMoved( ofMouseEventArgs & mouse ){
			mouseX=mouse.x;
			mouseY=mouse.y;
			mouseMoved(mouse.x,mouse.y);
		}
		virtual void mouseDragged( ofMouseEventArgs & mouse ){
			mouseX=mouse.x;
			mouseY=mouse.y;
			mouseDragged(mouse.x,mouse.y,mouse.button);
		}
		virtual void mousePressed( ofMouseEventArgs & mouse ){
			mouseX=mouse.x;
			mouseY=mouse.y;
			mousePressed(mouse.x,mouse.y,mouse.button);
		}
		virtual void mouseReleased(ofMouseEventArgs & mouse){
			mouseX=mouse.x;
			mouseY=mouse.y;
			mouseReleased(mouse.x,mouse.y,mouse.button);
		}
};



