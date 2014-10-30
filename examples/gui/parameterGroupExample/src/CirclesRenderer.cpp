/*
 * CirclesRenderer.cpp
 *
 *  Created on: 11/07/2012
 *      Author: arturo
 */

#include "CirclesRenderer.h"

CirclesRenderer::CirclesRenderer() {
	// TODO Auto-generated constructor stub

}


void CirclesRenderer::setup(string name){
	parameters.setName(name);
	parameters.add(size.set("size",10,0,100));
	parameters.add(number.set("number",2,1,20));
	parameters.add(position.set("position",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));

	color.set("color",ofColor(127),ofColor(0,0),ofColor(255));

	parameters.add(color);
	parameters.add(frameNum.set("frameNum",0));
}

void CirclesRenderer::draw(){
	// this will work only inside CirclesRenderer class
	// since it's a readonly property
	frameNum ++;
	ofSetColor(color);
	for(int i=0;i<number;i++){
		ofDrawCircle(position.get().x-size*((number-1)*0.5-i), position.get().y, size);
	}
}
