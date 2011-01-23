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
	if(polyline.size()) addVertex(polyline[0]);
}

void ofShape::addSubShape(const ofShape & shape){
	subShapes.push_back(shape);
	bNeedsTessellation = true;
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
	renderer->setShape(*this);
	renderer->draw();
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
