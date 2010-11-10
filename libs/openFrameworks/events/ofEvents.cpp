#include "ofEvents.h"
#include "ofAppRunner.h"

// core events instance & arguments
#ifdef OF_USING_POCO
	ofCoreEvents ofEvents;
	ofEventArgs voidEventArgs;
#endif
//------------------------------------------
void ofNotifySetup(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	
	if(ofAppPtr){
		ofAppPtr->setup();
	}
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.setup, voidEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyUpdate(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	
	if(ofAppPtr){
		ofAppPtr->update();
	}
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.update, voidEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyDraw(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	
	if(ofAppPtr){
		ofAppPtr->draw();
	}
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.draw, voidEventArgs );
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
		ofNotifyEvent( ofEvents.keyPressed, keyEventArgs );
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
		ofNotifyEvent( ofEvents.keyReleased, keyEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyMousePressed(int x, int y, int button){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofMouseEventArgs mouseEventArgs;
	
	if(ofAppPtr){
		ofAppPtr->mousePressed(x,y,button);
		ofAppPtr->mouseX = x;
		ofAppPtr->mouseY = y;
	}

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		mouseEventArgs.button = button;
		ofNotifyEvent( ofEvents.mousePressed, mouseEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyMouseReleased(int x, int y, int button){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofMouseEventArgs mouseEventArgs;
	
	if(ofAppPtr){
		ofAppPtr->mouseReleased(x,y,button);
		ofAppPtr->mouseReleased();
		ofAppPtr->mouseX = x;
		ofAppPtr->mouseY = y;
	}

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		mouseEventArgs.button = button;
		ofNotifyEvent( ofEvents.mouseReleased, mouseEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyMouseDragged(int x, int y, int button){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofMouseEventArgs mouseEventArgs;
	
	if(ofAppPtr){
		ofAppPtr->mouseDragged(x,y,button);
		ofAppPtr->mouseX = x;
		ofAppPtr->mouseY = y;
	}

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		mouseEventArgs.button = button;
		ofNotifyEvent( ofEvents.mouseDragged, mouseEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyMouseMoved(int x, int y){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	static ofMouseEventArgs mouseEventArgs;
	
	if(ofAppPtr){
		ofAppPtr->mouseMoved(x,y);
		ofAppPtr->mouseX = x;
		ofAppPtr->mouseY = y;
	}

	#ifdef OF_USING_POCO
		mouseEventArgs.x = x;
		mouseEventArgs.y = y;
		ofNotifyEvent( ofEvents.mouseMoved, mouseEventArgs );
	#endif
}

//------------------------------------------
void ofNotifyExit(){
	ofBaseApp * ofAppPtr = ofGetAppPtr();
	if(ofAppPtr){
		ofAppPtr->exit();
	}
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.exit, voidEventArgs );
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