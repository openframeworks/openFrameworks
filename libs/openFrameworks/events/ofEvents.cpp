#include "ofEvents.h"
#include "ofAppRunner.h"

// core events instance & arguments
#ifdef OF_USING_POCO
	ofCoreEvents 				ofEventsTmp;
	ofEventArgs					voidEventArgsTmp;
#endif

//------------------------------------------
void ofNotifySetup(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	
	if(ofAppPtr){
		ofAppPtr->setup();
	}
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEventsTmp.setup, voidEventArgsTmp );
	#endif
}

//------------------------------------------
void ofNotifyUpdate(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	
	if(ofAppPtr){
		ofAppPtr->update();
	}
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEventsTmp.update, voidEventArgsTmp );
	#endif
}

//------------------------------------------
void ofNotifyDraw(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	
	if(ofAppPtr){
		ofAppPtr->draw();
	}
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEventsTmp.draw, voidEventArgsTmp );
	#endif
}

//------------------------------------------
void ofNotifyKeyPressed(int key){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofKeyEventArgs keyEventArgs;

	if(ofAppPtr){
		ofAppPtr->keyPressed(key);
	}
	
	#ifdef OF_USING_POCO
		keyEventArgs.key = key;
		ofNotifyEvent( ofEventsTmp.keyPressed, keyEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyKeyReleased(int key){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofKeyEventArgs keyEventArgs;

	if(ofAppPtr){
		ofAppPtr->keyReleased(key);
	}
	
	#ifdef OF_USING_POCO
		keyEventArgs.key = key;
		ofNotifyEvent( ofEventsTmp.keyReleased, keyEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyMousePressed(int x, int y, int button){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofMouseEventArgs mouseEventArgs;
	
	if(ofAppPtr){
		ofAppPtr->mousePressed(x,y,button);
	}

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		mouseEventArgs.button = button;
		ofNotifyEvent( ofEventsTmp.mousePressed, mouseEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyMouseReleased(int x, int y, int button){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofMouseEventArgs mouseEventArgs;
	
	if(ofAppPtr){
		ofAppPtr->mouseReleased(x,y,button);
	}

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		mouseEventArgs.button = button;
		ofNotifyEvent( ofEventsTmp.mouseReleased, mouseEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyMouseDragged(int x, int y, int button){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofMouseEventArgs mouseEventArgs;
	
	if(ofAppPtr){
		ofAppPtr->mouseDragged(x,y,button);
	}

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		mouseEventArgs.button = button;
		ofNotifyEvent( ofEventsTmp.mouseDragged, mouseEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyMouseMoved(int x, int y){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofMouseEventArgs mouseEventArgs;
	
	if(ofAppPtr){
		ofAppPtr->mouseMoved(x,y);
	}

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		ofNotifyEvent( ofEventsTmp.mouseMoved, mouseEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyExit(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->exit();
	}
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEventsTmp.exit, voidEventArgsTmp );
	#endif
}

//------------------------------------------
void ofNotifyWindowResized(int width, int height){
	static ofResizeEventArgs resizeEventArgs;

	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->windowResized(width, height);
	}
	
	#ifdef OF_USING_POCO
		resizeEventArgs.width	= width;
		resizeEventArgs.height	= height;
		ofNotifyEvent( ofEvents.windowResized, resizeEventArgs );
	#endif
}