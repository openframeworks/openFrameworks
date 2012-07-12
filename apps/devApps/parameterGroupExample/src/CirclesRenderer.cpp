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


void CirclesRenderer::setup(){
	parameters.setName("parameters");
	parameters.add(size.set("size",10,0,100));
	parameters.add(number.set("number",2,1,20));

	color.setName("color");
	color.add(r.set("r",127,0,255));
	color.add(g.set("g",127,0,255));
	color.add(b.set("b",127,0,255));
	color.add(a.set("a",127,0,255));

	parameters.add(color);
}

void CirclesRenderer::draw(){
	ofSetColor(r,g,b,a);
	for(int i=0;i<number;i++){
		ofCircle(ofGetWidth()*0.5-size*((number-1)*0.5-i), ofGetHeight()*0.5, size);
	}
}
