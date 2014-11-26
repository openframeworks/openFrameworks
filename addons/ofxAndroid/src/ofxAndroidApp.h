/*
 * ofxAndroidApp.h
 *
 *  Created on: 07/07/2010
 *      Author: arturo
 */

#pragma once

#include "ofBaseApp.h"
#include "ofEvents.h"
#include "ofConstants.h"


enum ofxAndroidSwipeDir{
	OFX_ANDROID_SWIPE_UP    = 1,
	OFX_ANDROID_SWIPE_DOWN  = 2,
	OFX_ANDROID_SWIPE_LEFT  = 3,
	OFX_ANDROID_SWIPE_RIGHT = 4
};

enum ofxAndroidDragDir{
	OFX_ANDROID_DRAG_UP    = 5,
	OFX_ANDROID_DRAG_DOWN  = 6,
	OFX_ANDROID_DRAG_LEFT  = 7,
	OFX_ANDROID_DRAG_RIGHT = 8
};


class ofxAndroidApp: public ofBaseApp{
public:
	virtual void pause(){};
	virtual void stop(){};
	virtual void resume(){};
	virtual void reloadTextures(){}
	virtual void unloadTextures(){}

	virtual void touchDown(int x, int y, int id) {};
	virtual void touchMoved(int x, int y, int id) {};
	virtual void touchUp(int x, int y, int id) {};
	virtual void touchDoubleTap(int x, int y, int id) {};
	virtual void touchCancelled(int x, int y, int id) {};
	OF_DEPRECATED_MSG("swipe(ofxAndroidSwipeDir swipeDir, int id) is deprecated, please update to swipe(ofxAndroidSwipeDir swipeDir, int id, float velocityX, float velocityY)",virtual void swipe(ofxAndroidSwipeDir swipeDir, int id){};)

	virtual void touchDown(ofTouchEventArgs & touch) {
		touchDown(touch.x, touch.y, touch.id);
	};
	
	virtual void touchMoved(ofTouchEventArgs & touch) {
		touchMoved(touch.x, touch.y, touch.id);
	};
	
	virtual void touchUp(ofTouchEventArgs & touch) {
		touchUp(touch.x, touch.y, touch.id);
	};

	virtual void touchDoubleTap(ofTouchEventArgs & touch) {
		touchDoubleTap(touch.x, touch.y, touch.id);
	};
	
	virtual void touchCancelled(ofTouchEventArgs & touch){
		touchCancelled(touch.x, touch.y, touch.id);
	};
	
	virtual void swipe(ofxAndroidSwipeDir swipeDir, int id, float velocityX, float velocityY){
		swipe(swipeDir, id);
	};
	
	virtual void touchDragged(ofxAndroidDragDir swipeDir, int id, float dx, float dy) {

	};
	
	virtual bool backPressed(){
		return false;
	}

	virtual bool menuItemSelected(string menu_id_str){
		return false;
	}

	virtual bool menuItemChecked(string menu_id_str, bool checked){
		return false;
	}

	virtual void okPressed(){};
	virtual void cancelPressed(){};
	virtual void networkConnected(bool connected){};
};
