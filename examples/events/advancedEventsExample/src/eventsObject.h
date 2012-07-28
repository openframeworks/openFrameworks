/*
 * ofxEventsAddon.h
 *
 *  Created on: 17-dic-2008
 *      Author: art
 */

#ifndef OFXEVENTSADDON_H_
#define OFXEVENTSADDON_H_

#include "ofMain.h"
#include "ofEvents.h"

class eventsObject{
public:
	eventsObject(){
	    counter = 0;
	}

	void enable(){
	    ofAddListener(ofEvents().update, this, &eventsObject::update);
	}

	void disable(){
	    ofRemoveListener(ofEvents().update, this, &eventsObject::update);
	}

	void update(ofEventArgs & args){
		counter+=0.3;
		int intCounter=(int)counter;
		ofNotifyEvent(newFloatEvent,counter,this);
		ofNotifyEvent(newIntEvent,intCounter,this);
	}

	ofEvent<float> newFloatEvent;
	ofEvent<int> newIntEvent;

private:
	float counter;
};
#endif /* OFXEVENTSADDON_H_ */
