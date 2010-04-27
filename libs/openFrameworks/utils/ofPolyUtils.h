#ifndef OFPOLYUTILS_H_
#define OFPOLYUTILS_H_

#include "ofConstants.h"
#include "ofTypes.h"

bool ofInsidePoly(float x, float y, const vector<ofPoint> & poly);
bool ofInsidePoly(const ofPoint & p, const vector<ofPoint> & poly);

bool ofLineSegmentCrosses(ofPoint line1Start, ofPoint line1End, ofPoint line2Start, ofPoint line2End);
ofPoint ofLineSegmentIntersection(ofPoint line1Start, ofPoint line1End, ofPoint line2Start, ofPoint line2End);

ofPoint ofBezierPoint( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);
ofPoint ofCurvePoint( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);
ofPoint ofBezierTangent( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);
ofPoint ofCurveTangent( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);

#endif /* OFPOLYUTILS_H_ */
