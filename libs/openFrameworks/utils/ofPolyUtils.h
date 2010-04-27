#ifndef OFPOLYUTILS_H_
#define OFPOLYUTILS_H_

#include "ofConstants.h"
#include "ofTypes.h"

bool ofInsidePoly(float x, float y, const vector<ofPoint> & poly);
bool ofInsidePoly(const ofPoint & p, const vector<ofPoint> & poly);

#endif /* OFPOLYUTILS_H_ */
