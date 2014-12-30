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

class ofxAndroidApp: public ofBaseApp{
public:
	virtual void pause(){};
	virtual void stop(){};
	virtual void resume(){};
	virtual void reloadTextures(){}
	virtual void unloadTextures(){}
	virtual void swipe(ofxAndroidSwipeDir swipeDir, int id){

	}

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
