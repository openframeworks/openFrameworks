/*
 * ofxAndroidApp.h
 *
 *  Created on: 07/07/2010
 *      Author: arturo
 */

#ifndef OFXANDROIDAPP_H_
#define OFXANDROIDAPP_H_

#include "ofBaseApp.h"
#include "ofEvents.h"
#include "ofConstants.h"

class ofxAndroidApp: public ofBaseApp{
public:
	virtual void pause(){};
	virtual void stop(){};
	virtual void resume(){};
	virtual void reloadTextures(){}

	virtual void touchDown(int x, int y, int id) {};
	virtual void touchMoved(int x, int y, int id) {};
	virtual void touchUp(int x, int y, int id) {};
	virtual void touchDoubleTap(int x, int y, int id) {};
	virtual void touchCancelled(int x, int y, int id) {};

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
};


#endif /* OFXANDROIDAPP_H_ */
