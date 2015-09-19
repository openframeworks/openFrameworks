#pragma once
#include "ofPoint.h"
#include "ofConstants.h"
#include <deque>
#include "ofRectangle.h"

/// \file 
/// ofPolyLine allows you to combine multiple points into a single vector data
/// object that can be drawn to the screen, manipulated point by point, and
/// combined with other ofPolyline instances. It is less complex than the ofPath
/// and generally represents a single line or vector shape rather than multiple
/// lines or shapes.
/// 
/// You can add points to an ofPolyline by adding vertices:
/// ~~~~{.cpp}
/// float i = 0;
/// while (i < TWO_PI) { // make a heart
/// 	float r = (2-2*sin(i) + sin(i)*sqrt(abs(cos(i))) / (sin(i)+1.4)) * -80;
/// 	float x = ofGetWidth()/2 + cos(i) * r;
/// 	float y = ofGetHeight()/2 + sin(i) * r;
/// 	line.addVertex(ofVec2f(x,y));
/// 	i+=0.005*HALF_PI*0.5;
/// }
/// line.close(); // close the shape
/// ~~~~
///
/// Or you can draw lines or curves:
/// ~~~~{.cpp}
/// float angle = 0;
/// while (angle < TWO_PI ) {
/// 	b.curveTo(100*cos(angle), 0, 100*sin(angle));
/// 	b.curveTo(300*cos(angle), 300, 300*sin(angle));
/// 	angle += TWO_PI / 30;
/// }
/// ~~~~
/// 
/// ofPolyline also includes methods to get the closest point, determine whether a
/// point is inside shape, and resample shapes. Along with the ofPath class, it's
/// the best way to draw and manipulate 2D and 3D vector graphics that you'll need
/// to update and manipulate frequently.
/// 
/// If you use the lineTo() or curveTo() or bezierTo() functions, you move the
/// drawing point, so that drawing a line to 100,100 means a line from 0,0 to
/// 100, 100. The next line would be a line from 100,100 to wherever you go
/// next. Storing this position means that you can easily create continuous
/// drawings without difficulty.



class ofRectangle;

class ofPolyline {
public:
	/// \name Constructors
	/// \{

	/// \brief Creates an ofPolyline.
	ofPolyline();

	/// \brief Creates an ofPolyline from a vector of ofVec2f or ofPoint objects.
	ofPolyline(const vector<ofPoint>& verts);

    static ofPolyline fromRectangle(const ofRectangle& rect);

    /// \}
    /// \name Add and Remove Vertices
    /// \{

	/// \brief Removes all the points from the ofPolyline.
	void clear();

	/// \brief Adds a point using an ofPoint at the end of the ofPolyline.	
	void addVertex( const ofPoint& p );

    /// \brief Adds a point using floats at the end of the ofPolyline.
	void addVertex( float x, float y, float z=0 );

	/// \brief Add multiple points at the end of the ofPolyline using a vector of
	/// ofPoint objects
	/// 
	/// ~~~~{.cpp}
	/// 	vector<ofPoint> verts;
	/// 	// make a pentagon
	/// 	float size = 80.f;
	/// 	float X1 = 0.125*sqrt(10 + 2*sqrt(5)) * size;
	/// 	float X2 = 0.125*sqrt(10 - 2*sqrt(5)) * size;
	/// 	float Y1 = 0.125*(sqrt(5) - 1) * size;
	/// 	float Y2 = 0.125*(sqrt(5) + 1) * size;
	/// 	verts.push_back(ofPoint(0, -0.5 * size));
	/// 	verts.push_back(ofPoint(-X1, -Y1));
	/// 	verts.push_back(ofPoint(-X2, Y2));
	/// 	verts.push_back(ofPoint(X2, Y2));
	/// 	verts.push_back(ofPoint(X1, -Y1));
	/// 	ofPolyline p;
	/// 	p.addVertices(verts);
	/// ~~~~
	void addVertices( const vector<ofPoint>& verts );

	/// \brief Adds multiple points at the end of the ofPolyline using a pointer to
	/// an array of ofPoint objects.
	void addVertices(const ofPoint* verts, int numverts);

    void insertVertex(const ofPoint &p, int index);
    void insertVertex(float x, float y, float z, int index);

	/// \brief Resize the number of points in the ofPolyline to the value 
	/// passed in.
	void resize(size_t size);

    /// \}
    /// \name Access Vertices
	/// \{

	/// \brief The number of points in the ofPolyline.
	size_t size() const;
		
	/// Allows you to access the points of the ofPolyline just like you would
	/// in an array, so to make the points of a line follow the mouse
	/// movement, you could do:
	///
	/// ~~~~{.cpp}
	/// line[0].set(mouseX, mouseY);
	/// int i = 1;
	/// while ( i<bounds.size()) {
	/// 	float angle = atan2(line[i-1].y - line[i].y, line[i-1].x - line[i].x);  
	/// 	bounds[i].set(bounds[i-1].x - cos(angle) * 20, bounds[i-1].y - sin(angle) * 20);
	/// 	i++;
	/// }
	/// ~~~~
	const ofPoint& operator[] (int index) const;
	ofPoint& operator[] (int index);
    
	/// \brief Gets a vector of vertices that the line contains
	vector<ofPoint> & getVertices();
	const vector<ofPoint> & getVertices() const;

	vector<ofPoint>::iterator begin();
	vector<ofPoint>::const_iterator begin() const;
	vector<ofPoint>::reverse_iterator rbegin();
	vector<ofPoint>::const_reverse_iterator rbegin() const;
	vector<ofPoint>::iterator end();
	vector<ofPoint>::const_iterator end() const;
	vector<ofPoint>::reverse_iterator rend();
	vector<ofPoint>::const_reverse_iterator rend() const;

	/// \}
    /// \name Lines and Curves
    /// \{

	/// \brief Add a straight line from the last point added, or from 0,0 if no point
	/// is set, to the point indicated by the ofPoint passesd in.
	void lineTo(const ofPoint & to ){ addVertex(to); }
	
	/// \brief Add a straight line from the last point added, or from 0,0 if no point
	/// is set, to the point indicated by the floats x,y,z passesd in.
	void lineTo(float x, float y, float z=0){
		addVertex(x,y,z);
	}
    
    /// \brief Adds an arc around the ofPoint `center` with the width of `radiusX`
    /// and the height of `radiusY` to the polyline.
	/// 
	/// The `angleBegin` and `angleEnd` indicate the start and end angles of
	/// the arc in degrees measured clockwise from the x-axis.
	/// 
	/// The `clockwise` boolean sets the drawing direction.  Passing 'false' to
	/// it will draw the arc counter-clockwise.
	/// 
	/// Optionally, you can specify `circleResolution`, which is the number of
	/// line segments a circle would be drawn with.
	///
	/// If the arc doesn't start at the same point the last vertex finished a
	/// straight line will be created to join both
    void arc(const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise, int circleResolution = 20);
    
	/// \brief Adds an arc around the ofPoint `center` with the width of
	/// `radiusX` and the height of `radiusY`.
	///
	/// The `angleBegin` and `angleEnd` indicate the start and end angles
	/// of the arc in degrees measured clockwise from the x-axis.
	///  
	/// Optionally, you can specify `circleResolution`, which is the number of
	/// line segments a circle would be drawn with. A partial arc will be
	/// drawn with the same resolution: if circleResolution == 20, a half-
	/// circle will be drawn with 10 segments.	
	/// 
	/// If there are already vertexes in the ofPolyline the arc will extend
	/// them; a line will be created from the endmost point on the ofPolyline
	/// to the beginning point of the arc.	
	/// 
	/// ~~~~{.cpp}
	/// ofPolyline polyline1, polyline2;
	/// 
	/// // draw an line, then an semi-circle in red
	/// polyline2.lineTo(300, 50);
	/// ofPoint point2(450,120);
	/// polyline2.arc(point2,100,100,0,180);
	/// ofSetColor(ofColor::red);
	/// polyline2.draw();
	/// ~~~~
	/// 
	/// ![Arc Example](graphics/ofpolyline_arc.jpg)
	void arc(const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20) {
        arc(center, radiusX,  radiusY,  angleBegin,  angleEnd, true,  circleResolution);
    }

	/// \brief Adds an arc around the coordinates (`x`,`y`) with the width of
	/// `radiusX` and the height of `radiusY`.
	///
	/// The `angleBegin` and `angleEnd` indicate the start and end angles
	/// of the arc in degrees measured clockwise from the x-axis.
	/// 
	/// Optionally, you can specify `circleResolution`, which is the number
	/// of line segments a circle would be drawn with.
	void arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20){
		arc(ofPoint(x, y), radiusX, radiusY, angleBegin, angleEnd, true, circleResolution);
	}

	/// \brief Adds an arc around the coordinates (`x`,`y`,`z`) with the width of
	/// `radiusX` and the height of `radiusY`.
	/// 
	/// The `angleBegin` and `angleEnd` indicate the start and end angles of
	/// the arc in degrees measured clockwise from the x-axis.
	/// 
	/// Optionally, you can specify `circleResolution`, which is the number of
	/// line segments a circle would be drawn with.
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
    
    
	
	/// \brief Adds a curve to an ofPoint object passed in
	/// 
	/// ~~~~{.cpp}
	/// float angle = 0;
	/// while (angle < TWO_PI ) {
	/// 	b.curveTo( ofPoint(100*cos(angle), 100*sin(angle)));
	/// 	b.curveTo( ofPoint(300*cos(angle), 300*sin(angle)));
	/// 	angle += TWO_PI / 30;
	/// }
	/// ~~~~
	/// 
	/// \note You need at least 4 points to be able to use curveTo()
	/// \sa [Catmull-Rom splines wiki](http://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline)
	void curveTo( const ofPoint & to, int curveResolution = 20 );

	/// \brief Adds a curve to the x,y,z points passed in with the optional
	/// resolution.	
	void curveTo(float x, float y, float z = 0,  int curveResolution = 20 ){
		curveTo(ofPoint(x,y,z),curveResolution);
	}

	/// \brief Adds a cubic bezier line from the current drawing point with the 2
	/// control points indicated by ofPoint cp1 and cp2, that ends at ofPoint
	/// to. 
	///
	/// ~~~~{.cpp}
	/// line.addVertex(ofPoint(200, 400));
	/// line.bezierTo(100, 100, 800, 100, 700, 400);
	/// ~~~~
	/// ![polyline bezier](bezier.jpg)
	/// The control points are shown in red.
	void bezierTo( const ofPoint & cp1, const ofPoint & cp2, const ofPoint & to, int curveResolution = 20);

	/// \brief Adds a cubic bezier line from the current drawing point with the 2
	/// control points indicated by the coordinates cx1, cy1 and cx2, cy2,
	/// that ends at the coordinates x, y.
	void bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y, int curveResolution = 20){
		bezierTo(ofPoint(cx1,cy1),ofPoint(cx2,cy2),ofPoint(x,y),curveResolution);
	}

	/// \brief Adds a cubic bezier line in 3D space from the current drawing point
	/// with the 2 control points indicated by the coordinates cx1, cy1, cz1
	/// and cx2, cy2, cz2, that ends at the coordinates x, y, z.
	void bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z, int curveResolution = 20){
		bezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z),curveResolution);
	}

	/// \brief Adds a quadratic bezier line in 3D space from the current drawing
	/// point with the beginning indicated by the coordinates cx1, cy1, cz1,
	/// the control point at cx2, cy2, cz2, and that ends at the coordinates
	/// x, y, z.
	///
	/// ![polyline curves](graphics/curves.png)
	void quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z, int curveResolution = 20);

	/// \brief Adds a quadratic bezier line in 2D space from the current drawing
	/// point with the beginning indicated by the point p1, the control point
	/// at p2, and that ends at the point p3.
	void quadBezierTo(  const ofPoint & p1, const ofPoint & p2,const ofPoint & p3,  int curveResolution = 20 ){
		quadBezierTo(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,p3.x,p3.y,p3.z,curveResolution);
	}

	/// \brief Adds a quadratic bezier line in 2D space from the current drawing
	/// point with the beginning indicated by the coordinates cx1, cy1, the
	/// control point at cx2, cy2, and that ends at the coordinates x, y.
	void quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y, int curveResolution = 20){
		quadBezierTo(cx1,cy1,0,cx2,cy2,0,x,y,0,curveResolution);
	}

	/// \}
	/// \name Smoothing and Resampling
	/// \{

	/// \brief Gets a smoothed version of the ofPolyline. 
	/// 
	/// `smoothingSize` is the size of the smoothing window. So if
	/// `smoothingSize` is 2, then 2 points from the left, 1 in the center,
	/// and 2 on the right (5 total) will be used for smoothing each point.
	/// `smoothingShape` describes whether to use a triangular window (0) or
	/// box window (1) or something in between (for example, .5).
	ofPolyline getSmoothed(int smoothingSize, float smoothingShape = 0) const;

	/// \brief Resamples the line based on the spacing passed in. The larger the
	/// spacing, the more points will be eliminated.
	/// 
	/// ~~~~{.cpp}
	/// line.draw();
	/// ofTranslate(400, 0);
	/// line.getResampledBySpacing(100).draw();
	/// ~~~~
	/// ![polyline resample](graphics/resample.jpg)
	ofPolyline getResampledBySpacing(float spacing) const;

	/// \brief Resamples the line based on the count passed in. The lower the
	/// count passed in, the more points will be eliminated. 
	///
	/// This doesn't add new points to the line.
	ofPolyline getResampledByCount(int count) const;

    /// \brief Simplifies the polyline, removing un-necessary vertices. 
    ///
    /// \param tolerance determines how dis-similar points need to be to stay in the line.
    /// Higher tolerance means more points removed, lower tolerance means less
    /// points removed.
	void simplify(float tolerance=0.3f);

	/// \}
	/// \name Polyline State
	/// \{
	
	/// \brief Closes the ofPolyline, meaning that all the vertices will be linked
	/// and can be "walked".	
	void close();

	/// \brief Closes the ofPolyline, meaning that all the vertices will be linked
	/// and can be "walked".
	void setClosed( bool tf );
	bool isClosed() const;


	/// \brief Returns whether the vertices within the line have changed.
	bool hasChanged();
    void flagHasChanged();

    /// \}
	/// \name Geometric Functions
	/// \{

	/// \brief Tests whether the x,y coordinates are within a closed ofPolyline.
	static bool inside(float x, float y, const ofPolyline & polyline);
    /// \brief Tests whether the x,y coordinates are within a closed ofPolyline.
	bool inside(float x, float y) const;

	/// \brief Tests whether the ofPoint is within a closed ofPolyline.
	static bool inside(const ofPoint & p, const ofPolyline & polyline);
    /// \brief Tests whether the ofPoint is within a closed ofPolyline.
	bool inside(const ofPoint & p) const;
    
	/// \brief Get the bounding box of the polyline , taking into account 
	/// all the points to determine the extents of the polyline.
	ofRectangle getBoundingBox() const;
	
	/// \brief Gets the size of the perimeter of the polyline, good for
	/// determining length of the line, rather than just the bounding box
	/// shape.
	float getPerimeter() const;
	
	/// \brief Gets the precise area bounded by the line
	float getArea() const;

	/// \brief Get the center of the area bounded by the line
	ofPoint getCentroid2D() const;

	/// \brief Gets the point on the line closest to the target. You can also
	/// optionally pass a pointer to/address of an unsigned int to get the
	/// index of the closest vertex	
	ofPoint getClosestPoint(const ofPoint& target, unsigned int* nearestIndex = nullptr) const;
	

	/// \}
    /// \name Other Functions
    /// \{

    /// \brief Get (interpolated) index at given length along the path
    ///
    /// Includes info on percentage along segment, e.g. `ret=5.75` => 
    /// 75% along the path between 5th and 6th points
    float getIndexAtLength(float f) const;
    
    /// \brief Get (interpolated) index at given percentage along the path
    ///
    /// Includes info on percentage along segment, e.g. `ret=5.75` 
    /// => 75% along the path between 5th and 6th points
    float getIndexAtPercent(float f) const;

    /// \brief Get length along path at index
    float getLengthAtIndex(int index) const;
    
    /// \brief Get length along path at interpolated index (e.g. `f=5.75` => 75% along
    /// the path between 5th and 6th points)
    float getLengthAtIndexInterpolated(float findex) const;
    
    /// \brief Get point long the path at a given length (e.g. `f=150` => 150
    /// units along the path)    
    ofPoint getPointAtLength(float f) const;
    
    /// \brief Get point along the path at a given percentage (e.g. `f=0.25`
    /// => 25% along the path)
    ofPoint getPointAtPercent(float f) const;
    
    /// \brief Get point along the path at interpolated index (e.g. `f=5.75` =>
    /// 75% along the path between 5th and 6th points)
    ofPoint getPointAtIndexInterpolated(float findex) const;

    /// \brief Get angle (degrees) of the path at index
    float getAngleAtIndex(int index) const;
    
    /// \brief Get angle (degrees) at interpolated index (interpolated between
    /// neighboring indices)
    float getAngleAtIndexInterpolated(float findex) const;
    
    /// \brief Get rotation vector at index (magnitude is sin of angle)
    ofVec3f getRotationAtIndex(int index) const;
    
    /// \brief Get rotation vector at interpolated index 
    /// (interpolated between neighboring indices) (magnitude is sin of angle)
    ofVec3f getRotationAtIndexInterpolated(float findex) const;
    
    /// \brief Get tangent vector at index
    ofVec3f getTangentAtIndex(int index) const;
    
    /// \brief Get tangent vector at interpolated index 
   	/// (interpolated between neighboring indices)
    ofVec3f getTangentAtIndexInterpolated(float findex) const;

    /// \brief Get normal vector at index
    ofVec3f getNormalAtIndex(int index) const;
    
    /// \brief Get normal vector at interpolated index 
    /// (interpolated between neighboring indices)
    ofVec3f getNormalAtIndexInterpolated(float findex) const;
    
    /// \brief Get wrapped index depending on whether poly is closed or not
    int getWrappedIndex(int index) const;

    // used for calculating the normals
    void setRightVector(ofVec3f v = ofVec3f(0, 0, -1));
    ofVec3f getRightVector() const;

    /// \}
    /// \name Drawing
    /// \{
	
	/// \brief Draw the line using the current renderer
	void draw() const;
	
	/// \}
   
    
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

