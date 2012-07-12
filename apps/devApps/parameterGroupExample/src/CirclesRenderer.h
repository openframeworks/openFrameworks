/*
 * CirclesRenderer.h
 *
 *  Created on: 11/07/2012
 *      Author: arturo
 */

#ifndef CIRCLESRENDERER_H_
#define CIRCLESRENDERER_H_

#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofMain.h"

class CirclesRenderer {
public:
	CirclesRenderer();

	void setup();
	void draw();

	ofParameterGroup parameters;
	ofParameter<float> size;
	ofParameter<int> number;

	ofParameterGroup color;
	ofParameter<int> r,g,b,a;
};

#endif /* CIRCLESRENDERER_H_ */
