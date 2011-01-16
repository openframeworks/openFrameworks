/*
 *  ofShape.h
 *  openFrameworks
 *
 *  Created by theo on 28/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

#define DRAW_WITH_MESHIES

#ifdef DRAW_WITH_MESHIES
/// hack until ofMesh is available
typedef struct _meshy { GLint mode; vector<ofPoint>vertices; } meshy;
#else
#include "ofMesh.h"
#endif


/** ofPolyline
 
 A line composed of straight line segments.
 
 */


class ofPolyline {
public:	
	/// remove all the points
	void clear() { points.clear(); }

	/// add a vertex
	void addVertex( const ofPoint& p ) { points.push_back(p); }
	void addVertexes( const vector<ofPoint>& verts ) { points.insert( points.end(), verts.begin(), verts.end() ); }
	
	/// draw as line segments, with the current line style
	void draw() const;

	/// points vector access
	size_t size() const { return points.size(); }
	const ofPoint& operator[] (int index) const { return points[index]; }
	
	/// closed
	void setClosed( bool tf ) { bClosed = tf; }
	bool getClosed() const { return bClosed; }
	
private:
	vector<ofPoint> points;
	bool bClosed;
	
};



#include "ofTessellator.h"


/** ofShape
 
 Represents a 'shape'.
 
 */

class ofShape{
public:
	
	ofShape();
	void setCurveResolution(int numPoints);
	void clear();
	
	/// Add a vertex. Can be used to create straight lines or to specify start points for Bezier or
	/// Catmull-Rom curves.
	void addVertex(ofPoint p1);
	void addVertex( float x, float y, float z=0 ) 
		{ addVertex( ofPoint( x,y,z ) ); }
	
	/// Add a Bezier vertex by specifying ( control point out from previous point, control point in to 
	/// next point, next point ).
	void addBezierVertex(ofPoint cp1, ofPoint cp2, ofPoint p);
	void addBezierVertex( float cp1x, float cp1y, float cp2x, float cp2y, float px, float py )
		{ addBezierVertex( ofPoint(cp1x,cp1y), ofPoint(cp2x,cp2y), ofPoint(px,py) ); }
	void addBezierVertex( float cp1x, float cp1y, float cp1z, float cp2x, float cp2y, float cp2z, float px, float py, float pz )
		{ addBezierVertex( ofPoint(cp1x,cp1y,cp1z), ofPoint(cp2x,cp2y,cp2z), ofPoint(px,py,pz) ); }
	
	/// Add a Catmull-Rom curve vertex. You must add a minimum of vertices to make a Catmull-Rom spline, 
	/// and the first and last points will be used only as control points.
	void addCurveVertex(ofPoint p);
	void addCurveVertex( float x, float y, float z=0 ) 
		{ addCurveVertex( ofPoint( x,y,z ) ); }

	/// close the shape
	void close();
	/// next contour
	void nextContour( bool bClosePrev=true );

	/// must call tessellate before calling draw, if the shape has changed
	void tessellate();
	void draw();
	
	/// drawing style
	/// polygon winding mode for tessellation
	void setPolyWindingMode( int newMode );
	/// filled/outline
	void setFilled( bool bFill ) { bFilled = bFill; bNeedsTessellation = true; }
	/// set line + fill color simultaneously
	void setColor( const ofColor& color ) { setFillColor( color ); setLineColor( color ); }
	void setHexColor( int hex ) { setColor( ofColor().fromHex( hex ) ); };
	/// set line color
	void setLineColor( const ofColor& color ) { lineColor = color; }
	void setLineHexColor( int hex ) { setLineColor( ofColor().fromHex( hex ) ); };
	/// set fill color
	void setFillColor( const ofColor& color ) { fillColor = color; }
	void setFillHexColor( int hex ) { setFillColor( ofColor().fromHex( hex ) ); };
	
private:		
	
	typedef enum {
		OFSHAPE_SEG_LINE,
		OFSHAPE_SEG_BEZIER,
		OFSHAPE_SEG_CURVE
	} segmentType;
	
	class ofShapeSegment {
	public: 
		
		ofShapeSegment( segmentType _type ){
			type = _type;
		}
		
		/// up to you to call the correct function
		void addSegmentVertex(const ofPoint& p) {
			points.push_back(p)	;
		}
		void addSegmentCurveVertex(const ofPoint& p) {
			type = OFSHAPE_SEG_CURVE; points.push_back(p);
		}
		void addSegmentBezierVertex(const ofPoint& c1, const ofPoint& c2, const ofPoint& p) {
			type = OFSHAPE_SEG_BEZIER; points.push_back( c1 ); points.push_back( c2 ); points.push_back( p ); 
		}

		int getType() const { return type; }
		const vector<ofPoint>& getPoints() const { return points; }
		const ofPoint& getPoint( int index ) const { return points[index]; }
		size_t getNumPoints() const { return points.size(); }
	private:
		segmentType type;
		vector<ofPoint> points;
	};
	
	
	void curveSegmentToPolyline(const ofShapeSegment & seg, ofPolyline& polyline);
	void bezierSegmentToPolyline(const ofShapeSegment & seg, ofPolyline& polyline);
	
	ofColor lineColor;
	bool bFilled;
	ofColor fillColor;
	
	// true if this shape should be closed
	
	int resolution;
	vector<vector<ofShapeSegment> > segmentVectors;
	vector<bool> bShouldClose;
	
	int polyWindingMode;
	bool bNeedsTessellation;
	vector<ofPolyline> cachedPolylines;
	
	// resulting mesh and outline
	bool bNeedsOutlineDraw;
	ofPolyline cachedOutline;
	
#ifdef DRAW_WITH_MESHIES
	vector<meshy> cachedMeshies;
#else
	ofMesh cachedTessellation;
#endif
	
};


/** ofShapeCollection
 
 An ofShapeCollection holds one or more shapes. It knows its own position.
 
 @author Damian
 */
/*

class ofShapeCollection
{
public:
	
	
	/// add shapes to this collection
	void addShape( const ofShape& shape, ofPoint relativePosition = ofPoint( 0,0,0 ) );
	void addShape( const ofShapeCollection& collection, ofPoint relativePosition = ofPoint( 0,0,0 ) );
	
	/// set the position (relative to parent) of this shape collection
	void setPosition( ofPoint position );
	
	void draw();
	
private:
	
	vector<ofShapeCollection> children;
	
	ofPoint position;
	
};
*/



