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
#include "ofPolyline.h"

class ofPolyline;

//----------------------------------------------------------
// ofRectangle
//----------------------------------------------------------

class ofRectangle {
	
public:

    ofRectangle ();
    virtual ~ ofRectangle ();
    ofRectangle(float px, float py, float w, float h);
    ofRectangle(const ofPoint& p, float w, float h);
    ofRectangle(const ofRectangle & rect);
    ofRectangle(const ofPoint& p0, const ofPoint& p1);

    void set(float px, float py, float w, float h);
    void set(const ofPoint& p, float w, float h);
    void set(const ofRectangle& rect);
    void set(const ofPoint& p0, const ofPoint& p1);

    void setFromCenter(float px, float py, float w, float h);
    void setFromCenter(const ofPoint& p, float w, float h);

    ofPoint getCenter() const;

    void translate(float px, float py);
    void translate(const ofPoint& p);

    void scale(float s);
    void scale(float sX, float sY);
    void scale(const ofPoint& s);
    void scaleFromCenter(float s);
    void scaleFromCenter(float sX, float sY);
    void scaleFromCenter(const ofPoint& s);
    
    bool inside(float px, float py) const;
    bool inside(const ofPoint& p) const;
    bool inside(float px, float py, float w, float h) const;
    bool inside(const ofPoint& p, float w, float h) const;
    bool inside(const ofRectangle & rect) const;

    bool intersects(float px, float py, float w, float h) const;
    bool intersects(const ofPoint& p, float w, float h) const;
    bool intersects(const ofRectangle & rect) const;
    
    void growToInclude(float px, float py);
    void growToInclude(const ofPoint& p);
    void growToInclude(float px, float py, float w, float h);
    void growToInclude(const ofPoint&, float w, float h);
    void growToInclude(const ofRectangle& rect);
    void growToInclude(const ofPolyline& poly);

    ofRectangle getIntersection(float px, float py, float w, float h) const;
    ofRectangle getIntersection(const ofPoint& p, float w, float h) const;
    ofRectangle getIntersection(const ofRectangle& rect) const;

    ofRectangle getUnion(float px, float py, float w, float h) const;
    ofRectangle getUnion(const ofPoint& p, float w, float h) const;
    ofRectangle getUnion(const ofRectangle& rect) const;
    
    void canonicalize();
    ofRectangle getCanonicalized() const;
    bool isCanonicalized() const;  // are width/height >= 0.0f
    
    ofPolyline getAsPolyline() const;
    
    float getArea() const;
    float getPerimeter() const;
    
    bool isEmpty() const;     // are width/height == 0.0f
    
    ofPoint getMin() const;
    ofPoint getMax() const;
        
    float getMinX() const;
    float getMaxX() const;
    float getMinY() const;
    float getMaxY() const;
    
    ofRectangle & operator = (const ofRectangle& rect);
    ofRectangle & operator + (const ofPoint& p);

    bool operator == (const ofRectangle& rect) const;
    bool operator != (const ofRectangle& rect) const;
    
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    float x;
    float y;
    float width;
    float height;
};

