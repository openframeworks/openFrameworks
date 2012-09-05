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
                 ofRectScaleMode rectScaleMode = OF_RECTSCALEMODE_FIT);

    void scaleTo(const ofRectangle& targetRect,
                 ofAspectRatioMode subjectAspectRatioMode = OF_ASPECT_RATIO_KEEP,
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
    bool inside(const ofRectangle & rect) const;

    bool intersects(const ofRectangle & rect) const;
    
    void growToInclude(float px, float py);
    void growToInclude(const ofPoint& p);
    void growToInclude(const ofRectangle& rect);

    ofRectangle getIntersection(const ofRectangle& rect) const;

    ofRectangle getUnion(const ofRectangle& rect) const;
    
    void canonicalize();
    ofRectangle getCanonicalized() const;
    bool isCanonicalized() const;  // are width/height >= 0.0f
    
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
    
    float getHorzAnchor(ofAlignHorz anchor) const;
    float getVertAnchor(ofAlignVert anchor) const;
    
    ofPoint  getPosition() const;
    ofPoint& getPositionRef();

    ofPoint getCenter() const;
    
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    ofRectangle & operator = (const ofRectangle& rect);
    ofRectangle & operator + (const ofPoint& p);
    
    bool operator == (const ofRectangle& rect) const;
    bool operator != (const ofRectangle& rect) const;
    
    ofPoint position;
    float& x;
    float& y;
    float width;
    float height;
};

