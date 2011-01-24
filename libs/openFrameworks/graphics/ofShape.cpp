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
#include "ofShapeGLRenderers.h"
#include "ofPath.h"


/** 
 @TODO
	ofShape:
	- ofShapeCollection for multiple shapes inside (ofShape rename to ofPath)
	- ttf integration: ttf spits out ofShapeCollection
*/


ofShape::ofShape(){
	bFilled = ofGetStyle().bFill;
	bNeedsTessellation = true;
	polyWindingMode = ofGetStyle().polyMode;
	lineColor = ofGetStyle().color;
	fillColor = ofGetStyle().color;
	renderer = 0;
	clear();
}


ofShape::ofShape(const ofPath & path, int curveResolution, bool tesselate){
	setFrom(path,curveResolution,tesselate);
}

ofShape::~ofShape(){
	if(renderer)delete renderer;
}


void ofShape::clear() {
	bNeedsTessellation = true;
	polyline.clear();
	cachedTessellation.clear();
	subShapes.clear();
	bNeedsOutlineDraw = false;
}

void ofShape::close() {
	getCurrentSubShape().polyline.setClosed(true);
	newSubShape();
}

void ofShape::setPolyWindingMode( int newMode ) {
	polyWindingMode = newMode; 
	bNeedsTessellation = true; 
	// we shouldn't draw the tessellated outline if polyWindingMode is not ODD???
	//TODO: check but this seems from ofGraphics that it just needs to be tesselated in case
	// winding is != ODD if it is it just doesnt need to be tesselated.
}

void ofShape::tessellate(){

	bool bIs2D = !polyline.is3D();
	if(bFilled){
		cachedTessellation = ofTessellator::tessellateToMesh( getSubPolylines(), polyWindingMode, bIs2D );
	}
	if ( hasOutline() ){
		if( polyWindingMode != OF_POLY_WINDING_ODD ) {
			cachedOutline = ofTessellator::tessellateToOutline( getSubPolylines(), polyWindingMode, bIs2D );
		}else{
			cachedOutline = polyline;
		}
	}
	bNeedsTessellation = false;
}

void ofShape::draw(){
	ofSetColor( fillColor );
	if(!renderer){
		renderer=new OF_DEFAULT_SHAPE_RENDERER;
	}
	renderer->draw(*this);
}


void ofShape::addVertex(ofPoint p){
	polyline.addVertex(p);
	bNeedsTessellation = true;
}

void ofShape::setPolyline(const ofPolyline & polyline_){
	polyline = polyline_;
	bNeedsTessellation = true;
}

ofMesh & ofShape::getMesh(){
	if ( bNeedsTessellation ){
		tessellate();
	}
	return cachedTessellation;
}

vector<ofPolyline> ofShape::getSubPolylines(){
	vector<ofPolyline> polylines;
	polylines.push_back(polyline);
	for(int i=0;i<(int)subShapes.size();i++){
		vector<ofPolyline> subPolylines = subShapes[i].getSubPolylines();
		polylines.insert(polylines.end(),subPolylines.begin(),subPolylines.end());
	}
	return polylines;
}

//-------------------------------------------------------------------------
// primitive generation

ofShape &  ofShape::getCurrentSubShape(){
	if(subShapes.empty()){
		return *this;
	}else{
		return subShapes.back();
	}
}

ofShape & ofShape::newSubShape(){
	subShapes.push_back(ofShape());
	getCurrentSubShape().polyWindingMode = polyWindingMode;
	getCurrentSubShape().bFilled = bFilled;
	getCurrentSubShape().lineColor = lineColor;
	getCurrentSubShape().strokeWidth = strokeWidth;
	getCurrentSubShape().fillColor = fillColor;
	bNeedsTessellation = true;
	return subShapes.back();
}

void ofShape::addSubShape(const ofShape & shape){
	subShapes.push_back(shape);
	bNeedsTessellation = true;
}

void ofShape::setCircleResolution(int res){
	if (res > 1 && res != (int)circlePoints.size()){
		circlePoints.resize(res);

		float angle = 0.0f;
		float angleAdder = M_TWO_PI / (float)res;
		for (int i = 0; i < res; i++){
			circlePoints[i].x = cos(angle);
			circlePoints[i].y = sin(angle);
			circlePoints[i].z = 0.0f;
			angle += angleAdder;
		}
	}
}

void ofShape::setFrom(const ofPath & path,  int curveResolution, bool tesselate){
	// TODO: 3D commands
	clear();
	const vector<ofPath::Command> & commands = path.getCommands();
	for(int i=0; i<(int)commands.size();i++){
		switch(commands[i].type){
		case ofPath::Command::lineTo:
			curveVertices.clear();
			polyline.addVertex(commands[i].to);
			break;
		case ofPath::Command::curveTo:
			curveTo(commands[i].to, curveResolution);
			break;
		case ofPath::Command::bezier2DTo:
			curveVertices.clear();
			bezierTo(commands[i].cp1(),commands[i].cp2(),commands[i].to, curveResolution);
			break;
		case ofPath::Command::arc2D:
			curveVertices.clear();
			arc(commands[i].to,commands[i].radiusX(),commands[i].radiusY(),commands[i].angleBegin(),commands[i].angleEnd(), curveResolution);
			break;
		}
	}
	polyline.setClosed(path.isClosed());
	setFillColor(path.getFillColor());
	setStrokeColor(path.getStrokeColor());
	setPolyWindingMode(path.getWindingMode());
	setFilled(path.isFilled());
	//TODO: stroke width;


	for(int i=0; i<(int)path.getSubPaths().size(); i++){
		//TODO: cast to change the constness, nasty but ofPath is internally only changing the cached shape
		addSubShape(((ofPath&)path).getSubPaths()[i].getShape(curveResolution,tesselate));
	}
}

void ofShape::bezierTo( const ofPoint & cp1, const ofPoint & cp2, const ofPoint & to, int curveResolution ){
	// if, and only if poly vertices has points, we can make a bezier
	// from the last point
	curveVertices.clear();
	ofPolyline & polyline = getCurrentSubShape().polyline;

	// the resolultion with which we computer this bezier
	// is arbitrary, can we possibly make it dynamic?

	if (polyline.size() > 0){
		float x0 = polyline[polyline.size()-1].x;
		float y0 = polyline[polyline.size()-1].y;

		float   ax, bx, cx;
		float   ay, by, cy;
		float   t, t2, t3;
		float   x, y;

		// polynomial coefficients
		cx = 3.0f * (cp1.x - x0);
		bx = 3.0f * (cp2.x - cp1.x) - cx;
		ax = to.x - x0 - cx - bx;

		cy = 3.0f * (cp1.y - y0);
		by = 3.0f * (cp2.y - cp1.y) - cy;
		ay = to.y - y0 - cy - by;


		for (int i = 0; i < curveResolution; i++){
			t 	=  (float)i / (float)(curveResolution-1);
			t2 = t * t;
			t3 = t2 * t;
			x = (ax * t3) + (bx * t2) + (cx * t) + x0;
			y = (ay * t3) + (by * t2) + (cy * t) + y0;
			polyline.addVertex(x,y);
		}
	}
	bNeedsTessellation = true;
}



void ofShape::curveTo( const ofPoint & to, int curveResolution ){

	curveVertices.push_back(to);
	ofPolyline & polyline = getCurrentSubShape().polyline;

	if (curveVertices.size() == 4){

		int startPos = (int)curveVertices.size() - 4;

		float x0 = curveVertices[startPos + 0].x;
		float y0 = curveVertices[startPos + 0].y;
		float x1 = curveVertices[startPos + 1].x;
		float y1 = curveVertices[startPos + 1].y;
		float x2 = curveVertices[startPos + 2].x;
		float y2 = curveVertices[startPos + 2].y;
		float x3 = curveVertices[startPos + 3].x;
		float y3 = curveVertices[startPos + 3].y;

		float t,t2,t3;
		float x,y;

		for (int i = 0; i < curveResolution; i++){

			t 	=  (float)i / (float)(curveResolution-1);
			t2 	= t * t;
			t3 	= t2 * t;

			x = 0.5f * ( ( 2.0f * x1 ) +
			( -x0 + x2 ) * t +
			( 2.0f * x0 - 5.0f * x1 + 4 * x2 - x3 ) * t2 +
			( -x0 + 3.0f * x1 - 3.0f * x2 + x3 ) * t3 );

			y = 0.5f * ( ( 2.0f * y1 ) +
			( -y0 + y2 ) * t +
			( 2.0f * y0 - 5.0f * y1 + 4 * y2 - y3 ) * t2 +
			( -y0 + 3.0f * y1 - 3.0f * y2 + y3 ) * t3 );

			polyline.addVertex(ofPoint(x,y));
		}
		curveVertices.pop_front();
	}
	bNeedsTessellation = true;
}

void ofShape::arc( const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int curveResolution){
	setCircleResolution(curveResolution);
	ofPolyline & polyline = getCurrentSubShape().polyline;
	float size = (angleEnd - angleBegin)/360.0f;
	float begin = angleBegin/360.0f;

	if(size<1){
		const int segments = curveResolution*size;
		float angle,sinus,cosinus;
		float segment_size = M_PI*2.0*size/(float)segments;
		angle=-(M_PI*2.0*begin);
		for( int i=0; i<segments; i++){
			polyline.addVertex(radiusX*circlePoints[i].x+center.x,radiusY*circlePoints[i].y+center.y);
			angle-=segment_size ;
		}
		angle=-(M_PI*2.0*begin);
		sinus = sin(angle);
		cosinus = cos(angle);
		polyline.addVertex(radiusX*sinus+center.x,radiusY*cosinus+center.y);
	}else{
		for(int i=0;i<circlePoints.size();i++){
			polyline.addVertex(radiusX*circlePoints[i].x+center.x,radiusY*circlePoints[i].y+center.y);
		}
		polyline.addVertex(radiusX*circlePoints[0].x+center.x,radiusY*circlePoints[0].y+center.y);
	}
	bNeedsTessellation = true;
}
