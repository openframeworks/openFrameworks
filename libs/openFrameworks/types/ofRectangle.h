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
		
		ofRectangle ();
		virtual ~ ofRectangle ();
		ofRectangle (float _x, float _y, float _w, float _h);
		ofRectangle (ofRectangle const & r);
		
		void set (float px, float py, float w, float h);
		void set (ofPoint pos, float w, float h);
		void set (ofRectangle const & rect);
		void setFromCenter (float px, float py, float w, float h);
		void setFromCenter (ofPoint pos, float w, float h);
		
		ofPoint getCenter ();
		bool inside (ofPoint p);
		bool inside (float px, float py);
		
		ofRectangle & operator = (const ofRectangle & rect);
		bool operator == (ofRectangle const & r);
		bool operator != (ofRectangle const & r);
		ofRectangle & operator + (const ofPoint & point);
		
		float x;
		float y;
		float width;
		float height;
};

