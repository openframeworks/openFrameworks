/*
 *  ofShape.cpp
 *  openFrameworks
 *
 *  Created by theo on 28/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofShape.h"
#include "ofTessellator.h"



/** 
 @TODO
	ofShape:
	- ofShapeCollection for multiple shapes inside (ofShape rename to ofPath)
	- ttf integration: ttf spits out ofShapeCollection
*/


void ofPolyline::draw() const {
	for ( int i=1; i<(int)points.size(); i++ ) {
		ofLine( points[i-1], points[i] );
	}
}


ofShape::ofShape(){
	bFilled = ofGetStyle().bFill;
	resolution = 16;
	bNeedsTessellation = true;
	polyWindingMode = ofGetStyle().polyMode;
	lineColor = ofGetStyle().color;
	fillColor = ofGetStyle().color;
	clear();
}


void ofShape::clear() {
	segmentVectors.clear(); 
	segmentVectors.resize(1); 
	bShouldClose.clear(); 
	bShouldClose.resize(1);
	bNeedsTessellation = true;
	cachedPolylines.clear(); 
#ifdef DRAW_WITH_MESHIES
	cachedMeshies.clear();
#else
	cachedTessellation.clear();
#endif
	bNeedsOutlineDraw = false;
}


void ofShape::setCurveResolution(int numPoints) {
	resolution = numPoints;
}

void ofShape::close() {
	bShouldClose.back() = true;
}


void ofShape::nextContour( bool bClosePrev ){
	bShouldClose.back() = bClosePrev;
	segmentVectors.push_back( vector<ofShapeSegment>() );
	bShouldClose.push_back( false );
	bNeedsTessellation = true;
	// we shouldn't draw the tessellated outline if polyWindingMode is ODD.
	bNeedsOutlineDraw = (polyWindingMode!=OF_POLY_WINDING_ODD);
}


void ofShape::setPolyWindingMode( int newMode ) {
	polyWindingMode = newMode; 
	bNeedsTessellation = true; 
	// we shouldn't draw the tessellated outline if polyWindingMode is ODD.
	bNeedsOutlineDraw = bNeedsOutlineDraw && (polyWindingMode!=OF_POLY_WINDING_ODD);
}


void ofShape::bezierSegmentToPolyline( const ofShapeSegment & seg, ofPolyline& polyline ){

	
	// if, and only if poly vertices has points, we can make a bezier
	// from the last point
	
	// the resolultion with which we computer this bezier
	// is arbitrary, can we possibly make it dynamic?
	
	const vector<ofPoint>& points = seg.getPoints();
	if ( points.size()>= 4 ) {
		for ( int k=3; k<(int)points.size(); k+=3 ) {
			int k0 = k-3;
			int k1 = k-2;
			int k2 = k-1;
			int k3 = k;

			const ofPoint& p0 = points[k0];
			const ofPoint& p1 = points[k1];
			const ofPoint& p2 = points[k2];
			const ofPoint& p3 = points[k3];
			
			ofPoint a, b, c;
			float   t, t2, t3;
			
			// polynomial coefficients			
			c = 3.0f*(p1-p0);
			b = 3.0f*(p2-p1) - c;
			a = p3 - p0 - c - b;
			
			for (int i = 0; i < resolution; i++){
				t  = (float)i / (float)(resolution-1);
				t2 = t * t;
				t3 = t2 * t;
				ofPoint newPoint = a*t3 + b*t2 + c*t + p0;
				polyline.addVertex( newPoint );
			}
			
		}
	}	
}



void ofShape::curveSegmentToPolyline( const ofShapeSegment & seg, ofPolyline& polyline ){
	
	if( seg.getPoints().size() == 0 )
		return;
	
	ofPoint p0, p1, p2, p3;
	
	const vector<ofPoint>& p = seg.getPoints();
	
	if ( p.size() >= 4 ){	
		for(int k = 3; k < (int)p.size(); k++ ){
			float t,t2,t3;
			ofPoint p0 = p[k-3];
			ofPoint p1 = p[k-2];
			ofPoint p2 = p[k-1];
			ofPoint p3 = p[k  ];
			
			for (int i = 0; i < resolution; i++){
				t 	=  (float)i / (float)(resolution-1);
				t2 	= t * t;
				t3 	= t2 * t;

				ofPoint pt = 0.5f * ( (2.0f*p1) + (-p0+p2)*t + (2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3)*t2 + (-p0 + 3.0f*p1 - 3.0f*p2 + p3)*t3 );
				polyline.addVertex( pt );
			}
		}
	}
}

void ofShape::tessellate(){
	
//	ofLog(OF_LOG_NOTICE, "tessellate, %i segments", segments.size() );
	cachedPolylines.clear();
	
	if( segmentVectors.size()>0 ){
		cachedPolylines.resize( segmentVectors.size() );
		/// loop through all the subpaths
		for ( int h = 0; h < (int)segmentVectors.size(); h++ ) {
			/// loop through all the segments of this subpath
			const vector<ofShapeSegment>& segments = segmentVectors[h];
			ofPolyline& polyline = cachedPolylines[h];
			for(int i = 0; i < (int)segments.size(); i++){
				if( segments[i].getType() == OFSHAPE_SEG_LINE ){
					for(int j = 0; j < (int)segments[i].getPoints().size(); j++){
						polyline.addVertex( segments[i].getPoint(j) );
					}
				}else if( segments[i].getType() == OFSHAPE_SEG_BEZIER ){
					bezierSegmentToPolyline(segments[i], polyline);
				}else if( segments[i].getType() == OFSHAPE_SEG_CURVE ){
					curveSegmentToPolyline(segments[i], polyline);
				}
			}
			
			// close?
			polyline.setClosed( bShouldClose[h] );
			
		}
		
		bool bIs2D = false;
#ifdef DRAW_WITH_MESHIES
		cachedMeshies = ofTessellator::tessellateToMesh( cachedPolylines, polyWindingMode, bIs2D );
#else
		cachedTessellation = ofTessellator::tessellateToMesh( cachedPolylines, polyWindingMode, bIs2D );
#endif
		if ( bNeedsOutlineDraw ) {
			cachedOutline = ofTessellator::tessellateToOutline( cachedPolylines, polyWindingMode, bIs2D );
		}
	}
//	ofLog(OF_LOG_NOTICE, "tessellate done");
	
	bNeedsTessellation = false;
}

void ofShape::draw(){
	if ( bNeedsTessellation ){
		tessellate();
	}
	if ( bFilled ) {
#ifdef DRAW_WITH_MESHIES
		for ( int i=0; i<(int)cachedMeshies.size(); i++ ) {
			ofSetColor( fillColor );
			glBegin( cachedMeshies[i].mode );
			for ( int j=0; j<(int)cachedMeshies[i].vertices.size(); j++ ) {
				glVertex3f( cachedMeshies[i].vertices[j].x, cachedMeshies[i].vertices[j].y, cachedMeshies[i].vertices[j].z );
			}
			glEnd();
		}
#else
		cachedTessellation.drawVertices();
#endif
	}
	else {
				
		ofSetColor( lineColor );
		if ( bNeedsOutlineDraw ) {
			cachedOutline.draw();
		}
		else {
			for ( int i=0; i<(int)segmentVectors.size(); i++ ) {
				for ( int j=0; j<(int)segmentVectors[i].size(); j++ ) {
					for ( int k=1; k<(int)segmentVectors[i][j].getNumPoints(); k++ ) {
						ofLine( segmentVectors[i][j].getPoint(k-1), segmentVectors[i][j].getPoint(k) );
					}
					if ( bShouldClose[j] ) {
						// close the loop
						int last = segmentVectors[i][j].getNumPoints()-1;
						if ( last > 0 ) {
							ofLine( segmentVectors[i][j].getPoint(last), segmentVectors[i][j].getPoint(0) ); 
						}
					}
				}
			}
		}
		
	}
}


void ofShape::addVertex(ofPoint p){
	vector<ofShapeSegment>& segments = segmentVectors.back();
	if ( segments.size() == 0 ) {
		segments.push_back( ofShapeSegment( OFSHAPE_SEG_LINE ) );
	}
	else {
		ofShapeSegment& backSeg = segments.back();
		if ( backSeg.getType() != OFSHAPE_SEG_LINE ) {
			// back segment is not a line
			if ( backSeg.getNumPoints() > 0 ) {
				// back segment has points
				if ( backSeg.getType() == OFSHAPE_SEG_CURVE ) {
					// back segment is a Catmull-Rom, so we must add the new point to the old curve
					// to prevent a gap in the line
					backSeg.addSegmentCurveVertex( p );
				}
			}
			// push back a new line segment
			segments.push_back( ofShapeSegment( OFSHAPE_SEG_LINE ) );
		}
	}
	segments.back().addSegmentVertex( p );
	bNeedsTessellation = true;
}

void ofShape::addBezierVertex(ofPoint cp1, ofPoint cp2, ofPoint p){
	vector<ofShapeSegment>& segments = segmentVectors.back();
	if ( segments.size() == 0 || (segments.back().getType() != OFSHAPE_SEG_BEZIER && segments.back().getNumPoints()>1) ) {
		segments.push_back( ofShapeSegment( OFSHAPE_SEG_BEZIER ) );
		// user has done something stupid -- let's be kind
		segments.back().addSegmentVertex( cp1 );
	}
	segments.back().addSegmentBezierVertex( cp1, cp2, p );	
	bNeedsTessellation = true;
	bNeedsOutlineDraw = true;
}


void ofShape::addCurveVertex(ofPoint p){
	vector<ofShapeSegment>& segments = segmentVectors.back();
	if ( segments.size() == 0 ) {
		segments.push_back( ofShapeSegment( OFSHAPE_SEG_CURVE ) );
	}
	else {
		ofShapeSegment& backSeg = segments.back();
		if ( backSeg.getType() != OFSHAPE_SEG_CURVE ) {
			// back segment is not a curve
			// so we should start our new segment with the previous 2 points of the segment previous 
			// to us (for a line) or double up the last point (for a bezier segment)
			int ultimate, penultimate;
			ultimate = max(0,int(backSeg.getNumPoints())-1);
			if ( backSeg.getType() == OFSHAPE_SEG_BEZIER ) {
				penultimate = max(0,int(backSeg.getNumPoints())-2);
			}
			else { // OFSHAPE_SEG_LINE
				penultimate = max(0,int(backSeg.getNumPoints())-2);
			}
			ofShapeSegment newSeg = ofShapeSegment( OFSHAPE_SEG_CURVE );
			newSeg.addSegmentCurveVertex( backSeg.getPoints()[penultimate] );
			newSeg.addSegmentCurveVertex( backSeg.getPoints()[ultimate] );
			segments.push_back( newSeg );
		}
	}
	segments.back().addSegmentCurveVertex( p );	
	bNeedsTessellation = true;
	bNeedsOutlineDraw = true;
}

