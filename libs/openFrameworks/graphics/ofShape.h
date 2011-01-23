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

#define OF_DEFAULT_SHAPE_RENDERER ofVAShapeRenderer()

/** ofPolyline
 
 A line composed of straight line segments.
 
 */


class ofPolyline {
public:	
	ofPolyline(){
		bIs3D = false;
		bHasChanged = true;
	}
	/// remove all the points
	void clear() { points.clear(); }

	/// add a vertex
	void addVertex( const ofPoint& p ) { points.push_back(p); bHasChanged=true; }
	void addVertex( float x, float y ) { addVertex(ofPoint(x,y)); bHasChanged=true; }
	void addVertexes( const vector<ofPoint>& verts ) { points.insert( points.end(), verts.begin(), verts.end() );  bHasChanged=true;  }

	/// points vector access
	size_t size() const { return points.size(); }
	const ofPoint& operator[] (int index) const { return points[index]; }
	ofPoint& operator[] (int index) {  bHasChanged=true; return points[index]; }
	
	/// closed
	void setClosed( bool tf ) {  bHasChanged=true; bClosed = tf; }
	bool isClosed() const { return bClosed; }
	
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

	vector<ofPoint> & getVertices(){return points;}
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
	~ofShape();

	void clear();
	
	/// Add a vertex. Can be used to create straight lines or to specify start points for Bezier or
	/// Catmull-Rom curves.
	void addVertex(ofPoint p1);
	void addVertex( float x, float y, float z=0 ) 
		{ addVertex( ofPoint( x,y,z ) ); }

	void setPolyline(const ofPolyline & polyline);
	ofPolyline & getPolyline(){ return cachedOutline; };
	
	void addSubShape(const ofShape & shape);

	/// close the shape
	void close();

	/// next contour
	ofShape & newSubShape( bool bClosePrev=true );

	ofMesh & getMesh();
	const ofMesh & getMesh() const;

	/// must call tessellate before calling draw, if the shape has changed
	void tessellate();
	void draw();
	
	/// drawing style
	/// polygon winding mode for tessellation
	void setPolyWindingMode( int newMode );
	/// filled/outline
	void setFilled( bool bFill ) { bFilled = bFill; bNeedsTessellation = true; }
	bool isFilled() const { return bFilled; }
	/// set line + fill color simultaneously
	void setColor( const ofColor& color ) { setFillColor( color ); setLineColor( color ); }
	void setHexColor( int hex ) { setColor( ofColor().fromHex( hex ) ); };
	/// set line color
	void setLineColor( const ofColor& color ) { lineColor = color; }
	void setLineHexColor( int hex ) { setLineColor( ofColor().fromHex( hex ) ); };
	/// set fill color
	void setFillColor( const ofColor& color ) { fillColor = color; }
	void setFillHexColor( int hex ) { setFillColor( ofColor().fromHex( hex ) ); };
	
	bool hasOutline(){ return bNeedsOutlineDraw || !bFilled; }


private:
	vector<ofPolyline> getSubPolylines();
	// a shape is a polyline + lineColor, fill, windingMode and subshapes
	ofColor lineColor;
	bool bFilled;
	ofColor fillColor;
	int polyWindingMode;
	bool bNeedsTessellation;
	ofPolyline polyline;
	
	// resulting mesh and outline
	ofMesh cachedTessellation;
	bool bNeedsOutlineDraw;
	ofPolyline cachedOutline;
	
	vector<ofShape> subShapes;
	
	ofBaseShapeRenderer * renderer;

};
