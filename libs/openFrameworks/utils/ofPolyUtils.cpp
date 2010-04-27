/*
 * ofPolyUtils.cpp
 *
 *  Created on: 27/04/2010
 *      Author: arturo
 */

#include "ofPolyUtils.h"


bool ofInsidePoly(const ofPoint & p, const vector<ofPoint> & poly){
	return ofInsidePoly(p.x,p.y,poly);
}

bool ofInsidePoly(float x, float y, const vector<ofPoint> & polygon){
	int counter = 0;
	int i;
	double xinters;
	ofPoint p1,p2;

	int N = polygon.size();

	p1 = polygon[0];
	for (i=1;i<=N;i++) {
		p2 = polygon[i % N];
		if (y > MIN(p1.y,p2.y)) {
		  if (y <= MAX(p1.y,p2.y)) {
			if (x <= MAX(p1.x,p2.x)) {
			  if (p1.y != p2.y) {
				xinters = (y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
				if (p1.x == p2.x || x <= xinters)
				  counter++;
			  }
			}
		  }
		}
		p1 = p2;
	}

	if (counter % 2 == 0) return false;
	else return true;
}
