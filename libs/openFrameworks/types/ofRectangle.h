/*
 *  ofRectangle.h
 *  openFrameworksLib
 *
 *  Created by zachary lieberman on 1/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofConstants.h"
#include "ofPoint.h"
#include "ofLog.h"

//----------------------------------------------------------
// ofRectangle
//----------------------------------------------------------

// ofRectangle is a simple container for describing the position
// and size of 2D rectangles. Like many rectangle data structures
// found in other frameworks and graphics libraries, member
// width and height variables can take negative values.
// Additionally, x / y position and width / height are publicly
// accessible, resulting in great programming freedom.
// Consequently, two visually identical rectangles can be
// represented in the following ways:
//
// ofRectangle myRect(0,0,100,100);
//
// AND
//
// ofRectangle myRect(100,100,-100,-100);
//
// While both representations will yield the same visual
// results in all openFrameworks renderers, the results of
// some method operations that modify x / y / width / height
// (such as scaling) produce mathematically correct, but
// visually different results for each of the above
// representations.
//
// That said, most `getter` methods return the same results
// for both "standardized" and "non-standardized" rectangles.
// Simply stated, the user must be aware of the differences
// between these two visually equal but mathematically different
// representations.
//
// To ensure consistent results, users are encouraged to make
// sure rectangles are "standardized" before using operations
// that modify x / y / width / height.
//
// "Standardized" rectangles are rectangles whose width >= 0 and
// height >= 0.  The `void standardize()` method can be used
// to ensure that the rectangle is "standardized".

class ofRectangle {
	
public:

    ofRectangle();
    virtual ~ofRectangle();
    ofRectangle(float px, float py, float w, float h);
    ofRectangle(const ofPoint& p, float w, float h);
    ofRectangle(const ofRectangle& rect);
    ofRectangle(const ofPoint& p0, const ofPoint& p1);

    void set(float px, float py, float w, float h);
    void set(const ofPoint& p, float w, float h);
    void set(const ofRectangle& rect);
    void set(const ofPoint& p0, const ofPoint& p1);

    void setX(float px);
    void setY(float py);
    
    void setWidth(float w);
    void setHeight(float h);

    void setPosition(float px, float py);
    void setPosition(const ofPoint& p);
    
    void setFromCenter(float px, float py, float w, float h);
    void setFromCenter(const ofPoint& p, float w, float h);

    void translate(float dx, float dy);
    void translate(const ofPoint& dp);
    
    void translateX(float dx);
    void translateY(float dy);

    void scale(float s);
    void scale(float sX, float sY);
    void scale(const ofPoint& s);
    
    void scaleWidth(float  sX);
    void scaleHeight(float sY);
    
    void scaleFromCenter(float s);
    void scaleFromCenter(float sX, float sY);
    void scaleFromCenter(const ofPoint& s);
    
    void scaleTo(const ofRectangle& targetRect,
                 ofScaleMode scaleMode = OF_SCALEMODE_FIT);

    void scaleTo(const ofRectangle& targetRect,
                 ofAspectRatioMode subjectAspectRatioMode,
                 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
                 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);

    void scaleTo(const ofRectangle& targetRect,
                 ofAspectRatioMode subjectAspectRatioMode,
                 ofAlignHorz modelHorzAnchor,
                 ofAlignVert modelVertAnchor,
                 ofAlignHorz subjectHorzAnchor,
                 ofAlignVert subjectVertAnchor);

    void alignToHorz(const float& targetX,
                     ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER);

    void alignToHorz(const ofRectangle& targetRect,
                     ofAlignHorz sharedAnchor = OF_ALIGN_HORZ_CENTER);

    void alignToHorz(const ofRectangle& targetRect,
                     ofAlignHorz targetHorzAnchor,
                     ofAlignHorz thisHorzAnchor);

    void alignToVert(const float& targetY,
                     ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);

    void alignToVert(const ofRectangle& targetRect,
                     ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);

    void alignToVert(const ofRectangle& targetRect,
                     ofAlignVert targetVertAnchor,
                     ofAlignVert thisVertAnchor);

    void alignTo(const ofPoint& targetPoint,
                ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER,
                ofAlignVert thisVertAnchor = OF_ALIGN_VERT_CENTER);
    
    void alignTo(const ofRectangle& targetRect,
                 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
                 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);

    void alignTo(const ofRectangle& targetRect,
                 ofAlignHorz targetHorzAnchor,
                 ofAlignVert targetVertAnchor,
                 ofAlignHorz thisHorzAnchor,
                 ofAlignVert thisVertAnchor);

    bool inside(float px, float py) const;
    bool inside(const ofPoint& p) const;
    bool inside(const ofRectangle& rect) const;
    bool inside(const ofPoint& p0, const ofPoint& p1) const;

    bool intersects(const ofRectangle& rect) const;
    bool intersects(const ofPoint& p0, const ofPoint& p1) const;
    
    void growToInclude(float px, float py);
    void growToInclude(const ofPoint& p);
    void growToInclude(const ofRectangle& rect);
    void growToInclude(const ofPoint& p0, const ofPoint& p1);

    ofRectangle getIntersection(const ofRectangle& rect) const;

    ofRectangle getUnion(const ofRectangle& rect) const;
    
    void standardize();
    ofRectangle getStandardized() const;
    bool isStandardized() const;  // are width/height >= 0.0f
    
    float getArea() const;
    float getPerimeter() const;
    float getAspectRatio() const;
    
    bool isEmpty() const;     // are width/height == 0.0f
    
    ofPoint getMin() const;
    ofPoint getMax() const;
    
    float getMinX() const;
    float getMaxX() const;
    float getMinY() const;
    float getMaxY() const;
    
    float getLeft()   const;
    float getRight()  const;
    float getTop()    const;
    float getBottom() const;
    
    ofPoint getTopLeft() const;
    ofPoint getTopRight() const;
    ofPoint getBottomLeft() const;
    ofPoint getBottomRight() const;
    
    float getHorzAnchor(ofAlignHorz anchor) const;
    float getVertAnchor(ofAlignVert anchor) const;
    
    ofPoint  getPosition() const;
    ofPoint& getPositionRef();

    ofPoint getCenter() const;
    
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    ofRectangle& operator = (const ofRectangle& rect);
    ofRectangle operator + (const ofPoint& p);
    
    bool operator == (const ofRectangle& rect) const;
    bool operator != (const ofRectangle& rect) const;
    
    ofPoint position;
    float& x;
    float& y;
    float width;
    float height;
};

ostream& operator<<(ostream& os, const ofRectangle& rect);
istream& operator>>(istream& is, ofRectangle& rect);

