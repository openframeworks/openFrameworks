#pragma once
#include "ofPoint.h"
#include "ofConstants.h"
#include <deque>
#include "ofRectangle.h"

// ofPolyline
// A line composed of straight line segments.

class ofPolyline {
public:
	ofPolyline();
	ofPolyline(const vector<ofPoint>& verts);

	/// remove all the points
	void clear();

	/// add a vertex
	void addVertex( const ofPoint& p );
	void addVertex( float x, float y, float z=0 );
	void addVertexes( const vector<ofPoint>& verts );
	void addVertexes(const ofPoint* verts, int numverts);

	// adds a straight line to the polyline
	void lineTo(const ofPoint & to ){ addVertex(to); }
	void lineTo(float x, float y, float z=0){
		addVertex(x,y,z);
	}

	// adds an arc to the polyline
	// if the arc doesn't start at the same point
	// the last vertex finished a straight line will
	// be created to join both
	void arc( const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int curveResolution=20 );
	void arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd, int curveResolution=20){
		arc(ofPoint(x,y),radiusX,radiusY,angleBegin,angleEnd,curveResolution);
	}
	void arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd, int curveResolution=20){
		arc(ofPoint(x,y,z),radiusX,radiusY,angleBegin,angleEnd,curveResolution);
	}

	// catmull-rom curve
	void curveTo( const ofPoint & to, int curveResolution=16 );
	void curveTo(float x, float y, float z=0,  int curveResolution=16 ){
		curveTo(ofPoint(x,y,z),curveResolution);
	}

	/// cubic bezier
	void bezierTo( const ofPoint & cp1, const ofPoint & cp2, const ofPoint & to, int curveResolution = 16);
	void bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y, int curveResolution=16){
		bezierTo(ofPoint(cx1,cy1),ofPoint(cx2,cy2),ofPoint(x,y));
	}
	void bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z, int curveResolution=16){
		bezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z),curveResolution);
	}

	/// quadratic bezier (lower resolution than cubic)
	void quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z, int curveResolution=16);
	void quadBezierTo(  const ofPoint & p1, const ofPoint & p2,const ofPoint & p3,  int curveResolution=16 ){
		quadBezierTo(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,p3.x,p3.y,p3.z,curveResolution);
	}
	void quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y, int curveResolution=16){
		quadBezierTo(cx1,cy1,0,cx2,cy2,0,x,y,0,curveResolution);
	}

	ofPolyline getSmoothed(int smoothingSize, float smoothingShape = 0);

	// resample a polyline based on the distance between the points
	ofPolyline getResampledBySpacing(float spacing);

	// resample a polyline based on the total point count
	ofPolyline getResampledByCount(int count);

	// get the bounding box of a polyline
	ofRectangle getBoundingBox();
	
	// find the closest point 'target' on a polyline
	// optionally pass a pointer to/address of an unsigned int to get the index of the closest vertex
	ofPoint getClosestPoint(const ofPoint& target, unsigned int* nearestIndex = NULL);

	void simplify(float tolerance=0.3);

	/// points vector access
	size_t size() const;
	const ofPoint& operator[] (int index) const;
	ofPoint& operator[] (int index);
	void resize(size_t size);

	/// closed
	void setClosed( bool tf );
	bool isClosed() const;
	void close();

	bool hasChanged();

	vector<ofPoint> & getVertices();

	float getPerimeter() const;
	float getArea() const;
	ofPoint getCentroid2D() const;

	void draw();		

private:
	void setCircleResolution(int res);
	vector<ofPoint> points;

	deque<ofPoint> curveVertices;
	vector<ofPoint> circlePoints;

	bool bClosed;
	bool bHasChanged;
};

