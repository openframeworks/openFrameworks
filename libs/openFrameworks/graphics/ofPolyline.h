#pragma once
#include "ofPoint.h"
#include "ofConstants.h"
#include <deque>
#include "ofRectangle.h"

// ofPolyline
// A line composed of straight line segments.

class ofRectangle;

class ofPolyline {
public:
	ofPolyline();
	ofPolyline(const vector<ofPoint>& verts);

    static ofPolyline fromRectangle(const ofRectangle& rect);

	/// remove all the points
	void clear();

	/// add a vertex
	void addVertex( const ofPoint& p );
	void addVertex( float x, float y, float z=0 );
	void addVertices( const vector<ofPoint>& verts );
	void addVertices(const ofPoint* verts, int numverts);

	// adds a straight line to the polyline
	void lineTo(const ofPoint & to ){ addVertex(to); }
	void lineTo(float x, float y, float z=0){
		addVertex(x,y,z);
	}
    
    void insertVertex(const ofPoint &p, int index);
    void insertVertex(float x, float y, float z, int index);

	// adds an arc to the polyline
	// if the arc doesn't start at the same point
	// the last vertex finished a straight line will
	// be created to join both
    
    void arc(const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise, int circleResolution = 20);
    
	void arc(const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20) {
        arc(center, radiusX,  radiusY,  angleBegin,  angleEnd, true,  circleResolution);
    }
	void arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20){
		arc(ofPoint(x, y), radiusX, radiusY, angleBegin, angleEnd, true, circleResolution);
	}
	void arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20){
		arc(ofPoint(x, y, z), radiusX, radiusY, angleBegin, angleEnd, true, circleResolution);
	}
    void arcNegative(const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20) {
        arc(center, radiusX, radiusY, angleBegin, angleEnd, false, circleResolution);
    }
	void arcNegative(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20){
        arc(ofPoint(x,y), radiusX, radiusY, angleBegin, angleEnd, false, circleResolution);
    }
	void arcNegative(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20){
		arc(ofPoint(x, y, z), radiusX, radiusY, angleBegin, angleEnd, false, circleResolution);
    }
    
    
	// catmull-rom curve
	void curveTo( const ofPoint & to, int curveResolution = 20 );
	void curveTo(float x, float y, float z = 0,  int curveResolution = 20 ){
		curveTo(ofPoint(x,y,z),curveResolution);
	}

	/// cubic bezier
	void bezierTo( const ofPoint & cp1, const ofPoint & cp2, const ofPoint & to, int curveResolution = 20);
	void bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y, int curveResolution = 20){
		bezierTo(ofPoint(cx1,cy1),ofPoint(cx2,cy2),ofPoint(x,y),curveResolution);
	}
	void bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z, int curveResolution = 20){
		bezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z),curveResolution);
	}

	/// quadratic bezier (lower resolution than cubic)
	void quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z, int curveResolution = 20);
	void quadBezierTo(  const ofPoint & p1, const ofPoint & p2,const ofPoint & p3,  int curveResolution = 20 ){
		quadBezierTo(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,p3.x,p3.y,p3.z,curveResolution);
	}
	void quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y, int curveResolution = 20){
		quadBezierTo(cx1,cy1,0,cx2,cy2,0,x,y,0,curveResolution);
	}

	ofPolyline getSmoothed(int smoothingSize, float smoothingShape = 0) const;

	// resample a polyline based on the distance between the points
	ofPolyline getResampledBySpacing(float spacing) const;

	// resample a polyline based on the total point count
	ofPolyline getResampledByCount(int count) const;

	// get the bounding box of a polyline
	ofRectangle getBoundingBox() const;
	
	// find the closest point 'target' on a polyline
	// optionally pass a pointer to/address of an unsigned int to get the index of the closest vertex
	ofPoint getClosestPoint(const ofPoint& target, unsigned int* nearestIndex = NULL) const;
	
	// check wheteher a point is inside the area enclosed by the polyline
	static bool inside(float x, float y, const ofPolyline & polyline);
	static bool inside(const ofPoint & p, const ofPolyline & polyline);
    
    // non-static versions
    bool inside(float x, float y) const;
    bool inside(const ofPoint & p) const;

	void simplify(float tolerance=0.3f);

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
    void flagHasChanged();

	vector<ofPoint> & getVertices();
	const vector<ofPoint> & getVertices() const;

	float getPerimeter() const;
	float getArea() const;
	ofPoint getCentroid2D() const;

	void draw();
    
    // used for calculating the normals
    void setRightVector(ofVec3f v = ofVec3f(0, 0, -1));
    ofVec3f getRightVector() const;
    
    
    // get (interpolated) index at given length along the path
    // includes info on percentage along segment, e.g. ret=5.75 => 75% along the path between 5th and 6th points
    float getIndexAtLength(float f) const;
    
    // get (interpolated) index at given percentage along the path
    // includes info on percentage along segment, e.g. ret=5.75 => 75% along the path between 5th and 6th points
    float getIndexAtPercent(float f) const;

    // get length along path at index
    float getLengthAtIndex(int index) const;
    
    // get length along path at interpolated index (e.g. f=5.75 => 75% along the path between 5th and 6th points)
    float getLengthAtIndexInterpolated(float findex) const;
    
    // get point long the path at a given length (e.g. f=150 => 150 units along the path)
    ofPoint getPointAtLength(float f) const;
    
    // get point along the path at a given percentage (e.g. f=0.25 => 25% along the path)
    ofPoint getPointAtPercent(float f) const;
    
    // get point along the path at interpolated index (e.g. f=5.75 => 75% along the path between 5th and 6th points)
    ofPoint getPointAtIndexInterpolated(float findex) const;

    // get angle (degrees) at index
    float getAngleAtIndex(int index) const;
    
    // get angle (degrees) at interpolated index (interpolated between neighboring indices)
    float getAngleAtIndexInterpolated(float findex) const;
    
    // get rotation vector at index (magnitude is sin of angle)
    ofVec3f getRotationAtIndex(int index) const;
    
    // get rotation vector at interpolated index (interpolated between neighboring indices) (magnitude is sin of angle)
    ofVec3f getRotationAtIndexInterpolated(float findex) const;
    
    // get tangent vector at index
    ofVec3f getTangentAtIndex(int index) const;
    
    // get tangent vector at interpolated index (interpolated between neighboring indices)
    ofVec3f getTangentAtIndexInterpolated(float findex) const;

    // get normal vector at index
    ofVec3f getNormalAtIndex(int index) const;
    
    // get normal vector at interpolated index (interpolated between neighboring indices)
    ofVec3f getNormalAtIndexInterpolated(float findex) const;
    
    // get wrapped index depending on whether poly is closed or not
    int getWrappedIndex(int index) const;
    
private:
	void setCircleResolution(int res);
    float wrapAngle(float angleRad);

	vector<ofPoint> points;
    ofVec3f rightVector;
    
    // cache
    mutable vector<float> lengths;    // cumulative lengths, stored per point (lengths[n] is the distance to the n'th point, zero based)
    mutable vector<ofVec3f> tangents;   // tangent at vertex, stored per point
    mutable vector<ofVec3f> normals;    //
    mutable vector<ofVec3f> rotations;   // rotation between adjacent segments, stored per point (cross product)
    mutable vector<float> angles;    // angle (degrees) between adjacent segments, stored per point (asin(cross product))
    mutable ofPoint centroid2D;
    mutable float area;
    
    
	deque<ofPoint> curveVertices;
	vector<ofPoint> circlePoints;

	bool bClosed;
	bool bHasChanged;   // public API has access to this
    mutable bool bCacheIsDirty;   // used only internally, no public API to read
    
    void updateCache(bool bForceUpdate = false) const;
    
    // given an interpolated index (e.g. 5.75) return neighboring indices and interolation factor (e.g. 5, 6, 0.75)
    void getInterpolationParams(float findex, int &i1, int &i2, float &t) const;
    
    void calcData(int index, ofVec3f &tangent, float &angle, ofVec3f &rotation, ofVec3f &normal) const;
};

