/*
 *  ofRectangle.h
 *  openFrameworksLib
 *
 *  Created by zachary lieberman on 1/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */



#pragma once
#include "ofPoint.h"


//----------------------------------------------------------
// ofRectangle
//----------------------------------------------------------

class ofRectangle {
public:
	ofRectangle(){ x = y = width = height = 0.0f; };
	virtual ~ofRectangle(){}
	
	ofRectangle(float _x, float _y, float _w, float _h){
		x = _x;
		y = _y;
		width = _w;
		height = _h;
	}
	ofRectangle(const ofRectangle & r){
		x = r.x;
		y = r.y;
		width = r.width;
		height = r.height;
	}
	
	void set(float px, float py, float w, float h){
		x		= px;
		y		= py;
		width	= w;
		height	= h;
	}
	
	void set(ofPoint pos, float w, float h){
		x		= pos.x;
		y		= pos.y;
		width	= w;
		height	= h;
	}
	
	void set(const ofRectangle & rect){
		x		= rect.x;
		y		= rect.y;
		width	= rect.width;
		height	= rect.height;
	}
	
	void setFromCenter(float px, float py, float w, float h){
		x		= px - w*0.5f;
		y		= py - h*0.5f;
		width	= w;
		height	= h;
	}
	
	void setFromCenter(ofPoint pos, float w, float h){
		x		= pos.x - w*0.5f;
		y		= pos.y - h*0.5f;
		width	= w;
		height	= h;
	}
	
	ofPoint getCenter(){
		return ofPoint(x + width * 0.5f, y + height * 0.5f, 0);
	}
	
	bool inside (ofPoint p){
		return inside(p.x, p.y);
	}
	
	bool inside(float px, float py){
		if( px > x && py > y && px < x + width && py < y + height ){
		    return true;
		}
		return false;
	}
	
	//------ Operators:
	
	//Set
	ofRectangle & operator=( const ofRectangle& rect ){
		x = rect.x;
		y = rect.y;
		width = rect.width;
		height = rect.height;
		return *this;
	}
	
	//equality
	bool operator==( const ofRectangle& r ) {
		return (x == r.x) && (y == r.y) && (width == r.width) && (height == r.height);
	}
	
	//inequality
	bool operator!=( const ofRectangle& r ) {
		return (x != r.x) || (y != r.y) || (width != r.width) || (height != r.height);
	}
	
	float x;
	float y;
	float width;
	float height;
};

