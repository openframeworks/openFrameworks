/*
//  ofBox.h
//  extended by Jordi Puig from ofxRectangle.h
//	Created by zachary lieberman
*/

#pragma once

#include "ofConstants.h"
#include "ofPoint.h"
#include "ofLog.h"

//----------------------------------------------------------
// ofBox
//----------------------------------------------------------

// ofBox is a simple container for describing the position
// and size of 3D boxes. Like many boxes data structures
// found in other frameworks and graphics libraries, member
// width and height variables can take negative values.
// Additionally, x / y / z position and width / height / depth are publicly
// accessible, resulting in great programming freedom.
// Consequently, two visually identical boxes can be
// represented in the following ways:
//
// ofBox myRect(0,0,0,100,100,100);
//
// AND
//
// ofBox myRect(100,100,100,-100,-100,-100);
//
// While both representations will yield the same visual
// results in all openFrameworks renderers, the results of
// some method operations that modify x / y / z  / width / height / depth
// (such as scaling) produce mathematically corbox, but
// visually different results for each of the above
// representations.
//
// That said, most `getter` methods return the same results
// for both "standardized" and "non-standardized" boxes.
// Simply stated, the user must be aware of the differences
// between these two visually equal but mathematically different
// representations.
//
// To ensure consistent results, users are encouraged to make
// sure boxes are "standardized" before using operations
// that modify x / y / z / width / height / depth.
//
// "Standardized" boxes are boxes whose width >= 0 and
// height >= 0.  The `void standardize()` method can be used
// to ensure that the box is "standardized".

class ofBox {
	
public:
	
    ofBox();
    virtual ~ofBox();
    ofBox(float px, float py, float pz, float w, float h, float d);
    ofBox(const ofPoint& p, float w, float h, float d);
    ofBox(const ofBox& box);
    ofBox(const ofPoint& p0, const ofPoint& p1);
	
    void set(float px, float py, float pz, float w, float h, float d);
    void set(const ofPoint& p, float w, float h, float d);
    void set(const ofBox& box);
    void set(const ofPoint& p0, const ofPoint& p1);
	
    void setX(float px);
    void setY(float py);
    void setZ(float pz);
    
    void setWidth(float w);
    void setHeight(float h);
    void setDepth(float d);
	
    void setPosition(float px, float py, float pz);
    void setPosition(const ofPoint& p);
    
    void setFromCenter(float px, float py, float pz, float w, float h, float d);
    void setFromCenter(const ofPoint& p, float w, float h, float d);
	
    void translate(float dx, float dy, float dz);
    void translate(const ofPoint& dp);
    
    void translateX(float dx);
    void translateY(float dy);
    void translateZ(float dz);
	
    void scale(float s);
    void scale(float sX, float sY, float sZ);
    void scale(const ofPoint& s);
    
    void scaleWidth(float  sX);
    void scaleHeight(float sY);
    void scaleDepth(float sZ);
    
    void scaleFromCenter(float s);
    void scaleFromCenter(float sX, float sY, float sZ);
    void scaleFromCenter(const ofPoint& s);
    /*
    void scaleTo(const ofBox& targetRect,
                 ofScaleMode scaleMode = OF_SCALEMODE_FIT);
	
    void scaleTo(const ofBox& targetRect,
                 ofAspectRatioMode subjectAspectRatioMode,
                 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
                 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);
	
    void scaleTo(const ofBox& targetRect,
                 ofAspectRatioMode subjectAspectRatioMode,
                 ofAlignHorz modelHorzAnchor,
                 ofAlignVert modelVertAnchor,
                 ofAlignHorz subjectHorzAnchor,
                 ofAlignVert subjectVertAnchor);
	
    void alignToHorz(const float& targetX,
                     ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER);
	
    void alignToHorz(const ofBox& targetRect,
                     ofAlignHorz sharedAnchor = OF_ALIGN_HORZ_CENTER);
	
    void alignToHorz(const ofBox& targetRect,
                     ofAlignHorz targetHorzAnchor,
                     ofAlignHorz thisHorzAnchor);
	
    void alignToVert(const float& targetY,
                     ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);
	
    void alignToVert(const ofBox& targetRect,
                     ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);
	
    void alignToVert(const ofBox& targetRect,
                     ofAlignVert targetVertAnchor,
                     ofAlignVert thisVertAnchor);
	
    void alignTo(const ofPoint& targetPoint,
				 ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER,
				 ofAlignVert thisVertAnchor = OF_ALIGN_VERT_CENTER);
    
    void alignTo(const ofBox& targetRect,
                 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
                 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);
	
    void alignTo(const ofBox& targetRect,
                 ofAlignHorz targetHorzAnchor,
                 ofAlignVert targetVertAnchor,
                 ofAlignHorz thisHorzAnchor,
                 ofAlignVert thisVertAnchor);
	*/
    bool inside(float px, float py, float pz) const;
    bool inside(const ofPoint& p) const;
    bool inside(const ofBox& box) const;
    bool inside(const ofPoint& p0, const ofPoint& p1) const;
	
    bool intersects(const ofBox& box) const;
//    bool intersects(const ofPoint& p0, const ofPoint& p1) const;
    
    void growToInclude(float px, float py, float pz);
    void growToInclude(const ofPoint& p);
    void growToInclude(const ofBox& box);
    void growToInclude(const ofPoint& p0, const ofPoint& p1);
	
    ofBox getIntersection(const ofBox& box) const;
	
    ofBox getUnion(const ofBox& box) const;
    
    void standardize();
    ofBox getStandardized() const;
    bool isStandardized() const;  // are width/height >= 0.0f
    
    float getArea() const;
    float getVolume() const;
//    float getAspectRatio() const;
    
    bool isEmpty() const;     // are width/height == 0.0f
    
    ofPoint getMin() const;
    ofPoint getMax() const;
    
    float getMinX() const;
    float getMaxX() const;
    float getMinY() const;
    float getMaxY() const;
    float getMinZ() const;
    float getMaxZ() const;
    
    float getLeft()   const;
    float getRight()  const;
    float getTop()    const;
    float getBottom() const;
    float getFront()  const;
    float getBack()   const;
    
/*	ofPoint getTopLeft() const;
    ofPoint getTopRight() const;
    ofPoint getBottomLeft() const;
    ofPoint getBottomRight() const;
*/  
    float getHorzAnchor(ofAlignHorz anchor) const;
    float getVertAnchor(ofAlignVert anchor) const;
    
    ofPoint  getPosition() const;
    ofPoint& getPositionRef();
	
    ofPoint getCenter() const;
    
    float getX() const;
    float getY() const;
    float getZ() const;
    float getWidth() const;
    float getHeight() const;
    float getDepth() const;
	
    ofBox& operator = (const ofBox& box);
    ofBox operator + (const ofPoint& p);
    
    bool operator == (const ofBox& box) const;
    bool operator != (const ofBox& box) const;
    
    ofPoint position;
    float& x;
    float& y;
    float& z;
    float width;
    float height;
    float depth;
};

ostream& operator<<(ostream& os, const ofBox& box);
istream& operator>>(istream& is, ofBox& box);

