/*
 * ofPolyUtils.cpp
 *
 *  Created on: 27/04/2010
 *      Author: arturo
 */

#include "ofPolyUtils.h"

//--------------------------------------------------
bool ofInsidePoly(const ofPoint & p, const vector<ofPoint> & poly){
	return ofInsidePoly(p.x,p.y,poly);
}

//--------------------------------------------------
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

//--------------------------------------------------
bool ofLineSegmentCrosses(ofPoint line1Start, ofPoint line1End, ofPoint line2Start, ofPoint line2End){
	ofPoint diffLA, diffLB;
	float compareA, compareB;
	diffLA = line1End - line1Start;
	diffLB = line2End - line2Start;
	compareA = diffLA.x*line1Start.y - diffLA.y*line1Start.x;
	compareB = diffLB.x*line2Start.y - diffLB.y*line2Start.x;
	if (
		(
			( ( diffLA.x*line2Start.y - diffLA.y*line2Start.x ) < compareA ) xor
			( ( diffLA.x*line2End.y - diffLA.y*line2End.x ) < compareA )
		)
		&&
		(
			( ( diffLB.x*line1Start.y - diffLB.y*line1Start.x ) < compareB ) xor
			( ( diffLB.x*line1End.y - diffLB.y*line1End.x) < compareB )
		)
	)    return true;

	return false;
}

//--------------------------------------------------
ofPoint ofLineSegmentIntersection(ofPoint line1Start, ofPoint line1End, ofPoint line2Start, ofPoint line2End){
	float lDetLineA, lDetLineB, lDetDivInv;
	ofPoint lDiffLA, lDiffLB, intersection;

	lDetLineA = line1Start.x*line1End.y - line1Start.y*line1End.x;
	lDetLineB = line2Start.x*line2End.y - line2Start.y*line2End.x;
	lDiffLA = line1Start - line1End;
	lDiffLB = line2Start - line2End;
	lDetDivInv = 1 / ((lDiffLA.x*lDiffLB.y) - (lDiffLA.y*lDiffLB.x));
	intersection.x =  ((lDetLineA*lDiffLB.x) - (lDiffLA.x*lDetLineB)) * lDetDivInv ;
	intersection.y =  ((lDetLineA*lDiffLB.y) - (lDiffLA.y*lDetLineB)) * lDetDivInv ;

	return intersection;

}

//--------------------------------------------------
ofPoint ofBezierPoint( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t){
    float tp = 1.0 - t;
    return a*tp*tp*tp + b*3*t*tp*tp + c*3*t*t*tp + d*t*t*t;
}

//--------------------------------------------------
ofPoint ofCurvePoint( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t){
    ofPoint pt;
    float t2 = t * t;
    float t3 = t2 * t;
    pt.x = 0.5f * ( ( 2.0f * b.x ) +
                   ( -a.x + c.x ) * t +
                   ( 2.0f * a.x - 5.0f * b.x + 4 * c.x - d.x ) * t2 +
                   ( -a.x + 3.0f * b.x - 3.0f * c.x + d.x ) * t3 );
    pt.y = 0.5f * ( ( 2.0f * b.y ) +
                   ( -a.y + c.y ) * t +
                   ( 2.0f * a.y - 5.0f * b.y + 4 * c.y - d.y ) * t2 +
                   ( -a.y + 3.0f * b.y - 3.0f * c.y + d.y ) * t3 );
    return pt;
}

//--------------------------------------------------
ofPoint ofBezierTangent( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t){
    return (d-a-c*3+b*3)*(t*t)*3 + (a+c-b*2)*t*6 - a*3+b*3;
}

//--------------------------------------------------
ofPoint ofCurveTangent( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t){
    ofPoint v0 = ( c - a )*0.5;
    ofPoint v1 = ( d - b )*0.5;
    return ( b*2 -c*2 + v0 + v1)*(3*t*t) + ( c*3 - b*3 - v1 - v0*2 )*( 2*t) + v0;

}
