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

class CirclesRenderer;

template<typename ParameterType>
class ofCirclesRendererParam: public ofReadOnlyParameter<ParameterType,CirclesRenderer>{
	friend class CirclesRenderer;
};

class CirclesRenderer {
public:
	CirclesRenderer();

	void setup(string name);
	void draw();

	ofParameterGroup parameters;
	ofParameter<float> size;
	ofParameter<int> number;
	ofParameter<glm::vec2> position;

	ofParameter<ofColor> color;

	ofCirclesRendererParam<int> frameNum;
};

#endif /* CIRCLESRENDERER_H_ */
