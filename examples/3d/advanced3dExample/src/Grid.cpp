#include "Grid.h"

// grid is a custom 3D
//  object that we've
//  created in this
//  example.

void grid::customDraw(){
	ofPushStyle();

	//give a saturation and lightness
	ofSetColor(255, 100, 100);

	ofDrawGrid(100.0f);

	ofPopStyle();
}
