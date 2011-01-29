/*
 *  ofShape.h
 *  openFrameworks
 *
 *  Created by theo on 28/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMesh.h"
#include "ofBaseTypes.h"
#include "ofPoint.h"
#include <deque>

/** ofPolyline
 
 A line composed of straight line segments.
 
 */


class ofPolyline {
public:	
	ofPolyline(){
		bIs3D = true;
		bHasChanged = true;
		bClosed=false;
	}

	ofPolyline(const vector<ofPoint>& verts){
		bIs3D = true;
		bHasChanged = true;
		bClosed=false;
		addVertexes(verts);
	}
	/// remove all the points
	void clear() { points.clear(); }

	/// add a vertex
	void addVertex( const ofPoint& p ) { points.push_back(p); bHasChanged=true; }
	void addVertex( float x, float y, float z=0 ) { addVertex(ofPoint(x,y,z)); bHasChanged=true; }
	void addVertexes( const vector<ofPoint>& verts ) { points.insert( points.end(), verts.begin(), verts.end() );  bHasChanged=true;  }

	/// points vector access
	size_t size() const { return points.size(); }
	const ofPoint& operator[] (int index) const { return points[index]; }
	ofPoint& operator[] (int index) {  bHasChanged=true; return points[index]; }
	
	/// closed
	void setClosed( bool tf ) {  bHasChanged=true; bClosed = tf; }
	bool isClosed() const { return bClosed; }
	
	void simplify(float tolerance=0.3);

	void setIs3D(bool bIs3D_){
		bHasChanged=true;
		bIs3D = bIs3D_;
	}
	bool is3D() const{
		return bIs3D;
	}
	bool hasChanged(){
		 if(bHasChanged){
			 bHasChanged=false;
		 	 return true;
		 }else{
			 return false;
		 }
	}

	const vector<ofPoint> & getVertices() const{return points;}
	bool getClosed() const { return bClosed; }
	
	float getPerimeter() const;
	
private:
	vector<ofPoint> points;
	bool bClosed;
	bool bIs3D;
	bool bHasChanged;
};



#include "ofTessellator.h"


/** ofShape
 
 Represents a 'shape'.
 
 */

class ofShape{
public:
	
	ofShape();
	ofShape(const ofPath & path, int curveResolution=16, bool tesselate=false);
	~ofShape();

	void clear();
	
	/// Add a vertex. Can be used to create straight lines or to specify start points for Bezier or
	/// Catmull-Rom curves.
	void addVertex(ofPoint p1);
	void addVertex( float x, float y, float z=0 ) 
		{ getCurrentSubShape().addVertex( ofPoint( x,y,z ) ); }

	vector<ofPolyline> & getOutline();
	
	void addSubShape(const ofShape & shape);

	/// close the shape
	void close();

	vector<ofVertexData> & getTessellation();

	/// must call tessellate before calling draw, if the shape has changed
	void tessellate();
	void simplify(float tolerance=0.3);
	void draw(float x=0, float y=0);
	
	/// drawing style
	/// polygon winding mode for tessellation
	void setPolyWindingMode( int newMode );
	/// filled/outline
	void setFilled( bool bFill );
	bool isFilled() const { return bFilled; }
	/// set line + fill color simultaneously
	void setColor( const ofColor& color ) { setFillColor( color ); setStrokeColor( color ); }
	void setHexColor( int hex ) { setColor( ofColor().fromHex( hex ) ); };
	/// set line color
	void setStrokeColor( const ofColor& color ) { lineColor = color; }
	void setStrokeHexColor( int hex ) { setStrokeColor( ofColor().fromHex( hex ) ); };
	ofColor getStrokeColor(){return lineColor;}

	void setStrokeWidth( float width ) {
		strokeWidth=width;
	}
	float getStrokeWidth(){return strokeWidth;}

	/// set fill color
	void setFillColor( const ofColor& color ) { fillColor = color; }
	void setFillHexColor( int hex ) { setFillColor( ofColor().fromHex( hex ) ); };
	ofColor getFillColor(){return fillColor;}
	
	bool hasOutline() const { return bNeedsOutlineDraw || !bFilled; }

	//any combination of this won't work in all cases, if you need complex shapes
	//create several objects or use a path and create a shape from it
	ofShape & newSubShape();
	void moveTo(const ofPoint & to ){ addVertex(to); }
	void moveTo(float x, float y, float z=0){
		addVertex(x,y,z);
	}
	void lineTo(const ofPoint & to ){ addVertex(to); }
	void lineTo(float x, float y, float z=0){
		addVertex(x,y,z);
	}
	void arc( const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int curveResolution=16 );
	void arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd, int curveResolution=16){
		arc(ofPoint(x,y),radiusX,radiusY,angleBegin,angleEnd,curveResolution);
	}
	void arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd, int curveResolution=16){
		arc(ofPoint(x,y,z),radiusX,radiusY,angleBegin,angleEnd,curveResolution);
	}
	void curveTo( const ofPoint & to, int curveResolution=16 );
	void curveTo(float x, float y, float z=0,  int curveResolution=16 ){
		curveTo(ofPoint(x,y,z),curveResolution);
	}

	/// cuadric bezier
	void bezierTo( const ofPoint & cp1, const ofPoint & cp2, const ofPoint & to, int curveResolution = 16);
	void bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y, int curveResolution=16){
		bezierTo(ofPoint(cx1,cy1),ofPoint(cx2,cy2),ofPoint(x,y));
	}
	void bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z, int curveResolution=16){
		bezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z),curveResolution);
	}

	/// cubic bezier
	void quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z, int curveResolution=16);
	void quadBezierTo(  const ofPoint & p1, const ofPoint & p2,const ofPoint & p3,  int curveResolution=16 ){
		quadBezierTo(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,p3.x,p3.y,p3.z,curveResolution);
	}
	void quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y, int curveResolution=16){
		quadBezierTo(cx1,cy1,0,cx2,cy2,0,x,y,0,curveResolution);
	}

	void setFrom(const ofPath & path,  int curveResolution=16, bool tesselate=false);
	void setPolyline(const ofPolyline & polyline);
private:
	vector<ofPolyline> & getSubPolylines();
	void setCircleResolution(int res);
	ofShape &  getCurrentSubShape();

	// a shape is a polyline + lineColor, fill, windingMode and subshapes
	ofColor lineColor;
	float strokeWidth;
	bool bFilled;
	ofColor fillColor;
	int polyWindingMode;
	bool bNeedsTessellation;
	ofPolyline polyline;
	
	// resulting mesh and outline
	vector<ofVertexData> cachedTessellation;
	bool bNeedsOutlineDraw;
	vector<ofPolyline> cachedOutline;
	
	vector<ofShape> subShapes;

	ofBaseRenderer * renderer;

	deque<ofPoint> curveVertices;
	vector<ofPoint> circlePoints;

	bool bIs3D;
};
