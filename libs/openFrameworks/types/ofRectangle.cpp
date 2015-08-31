#include <cfloat>
#include "ofRectangle.h"

//----------------------------------------------------------
ofRectangle::ofRectangle() : x(position.x), y(position.y) {
    set(0,0,0,0);
}

//----------------------------------------------------------
ofRectangle::~ ofRectangle(){}

//----------------------------------------------------------
ofRectangle::ofRectangle(float px, float py, float w, float h) : x(position.x), y(position.y) {
	set(px,py,w,h);
}

//----------------------------------------------------------
ofRectangle::ofRectangle(const ofPoint& p, float w, float h) : x(position.x), y(position.y) {
    set(p,w,h);
}

//----------------------------------------------------------
ofRectangle::ofRectangle(const ofRectangle& rect) : x(position.x), y(position.y) {
    set(rect);
}

//----------------------------------------------------------
ofRectangle::ofRectangle(const ofPoint& p0, const ofPoint& p1) : x(position.x), y(position.y) {
    set(p0,p1);
}

//----------------------------------------------------------
void ofRectangle::set(float px, float py, float w, float h) {
	x		= px;
	y		= py;
	width	= w;
	height	= h;
}

//----------------------------------------------------------
void ofRectangle::set(const ofPoint& p, float w, float h) {
    set(p.x, p.y, w, h);
}

//----------------------------------------------------------
void ofRectangle::set(const ofRectangle& rect){
    set(rect.x, rect.y, rect.width, rect.height);
}

//----------------------------------------------------------
void ofRectangle::set(const ofPoint& p0, const ofPoint& p1) {
    float x0,y0,x1,y1;

    x0 = MIN(p0.x, p1.x);
    x1 = MAX(p0.x, p1.x);
    y0 = MIN(p0.y, p1.y);
    y1 = MAX(p0.y, p1.y);

    float w = x1 - x0;
    float h = y1 - y0;

    set(x0,y0,w,h);
}

//----------------------------------------------------------
void ofRectangle::setX(float px) {
    x = px;
}

//----------------------------------------------------------
void ofRectangle::setY(float py) {
    y = py;
}

//----------------------------------------------------------
void ofRectangle::setWidth(float w) {
    width = w;
}

//----------------------------------------------------------
void ofRectangle::setHeight(float h) {
    height = h;
}

//----------------------------------------------------------
void ofRectangle::setPosition(float px, float py) {
    position.x = px;
    position.y = py;
}

//----------------------------------------------------------
void ofRectangle::setPosition(const ofPoint& p) {
    position = p;
}

//----------------------------------------------------------
void ofRectangle::setSize(float w, float h) {
	width = w;
	height = h;
}


//----------------------------------------------------------
void ofRectangle::setFromCenter(float px, float py, float w, float h) {
    set(px - w*0.5f, py - h*0.5f, w, h);
}

//----------------------------------------------------------
void ofRectangle::setFromCenter(const ofPoint& p, float w, float h) {
    setFromCenter(p.x, p.y, w, h);
}

//----------------------------------------------------------
void ofRectangle::translate(float dx, float dy) {
    translateX(dx);
    translateY(dy);
}

//----------------------------------------------------------
void ofRectangle::translate(const ofPoint& dp) {
    translateX(dp.x);
    translateY(dp.y);
}

//----------------------------------------------------------
void ofRectangle::translateX(float dx) {
    x += dx;
}

//----------------------------------------------------------
void ofRectangle::translateY(float dy) {
    y += dy;
}

//----------------------------------------------------------
void ofRectangle::scale(float s) {
    scaleWidth(s);
    scaleHeight(s);
}

//----------------------------------------------------------
void ofRectangle::scale(float sX, float sY) {
    scaleWidth(sX);
    scaleHeight(sY);
}

//----------------------------------------------------------
void ofRectangle::scale(const ofPoint& s) {
    scaleWidth(s.x);
    scaleHeight(s.y);
}

//----------------------------------------------------------
void ofRectangle::scaleWidth(float  sX) {
    width  *= sX;
}
//----------------------------------------------------------
void ofRectangle::scaleHeight(float sY) {
    height *= sY;
}

//----------------------------------------------------------
void ofRectangle::scaleFromCenter(float s) {
    scaleFromCenter(s,s);
}

//----------------------------------------------------------
void ofRectangle::scaleFromCenter(float sX, float sY) {
    scaleFromCenter(ofPoint(sX,sY));
}

//----------------------------------------------------------
void ofRectangle::scaleFromCenter(const ofPoint& s) {
    if(s.x == 1.0f && s.y == 1.0f) return; // nothing to do

    float newWidth  = width  * s.x;
    float newHeight = height * s.y;

    ofPoint center = getCenter();

    x = center.x - newWidth  / 2.0f;
    y = center.y - newHeight / 2.0f;

    width  = newWidth;
    height = newHeight;
}

//----------------------------------------------------------
void ofRectangle::scaleTo(const ofRectangle& targetRect,
                          ofScaleMode scaleMode) {

    if(scaleMode == OF_SCALEMODE_FIT) {
        scaleTo(targetRect,
                OF_ASPECT_RATIO_KEEP,
                OF_ALIGN_HORZ_CENTER,
                OF_ALIGN_VERT_CENTER);
    } else if(scaleMode == OF_SCALEMODE_FILL) {
        scaleTo(targetRect,
                OF_ASPECT_RATIO_KEEP_BY_EXPANDING,
                OF_ALIGN_HORZ_CENTER,
                OF_ALIGN_VERT_CENTER);
    } else if(scaleMode == OF_SCALEMODE_CENTER) {
        alignTo(targetRect,
                OF_ALIGN_HORZ_CENTER,
                OF_ALIGN_VERT_CENTER);
    } else if(scaleMode == OF_SCALEMODE_STRETCH_TO_FILL) {
        scaleTo(targetRect,
                OF_ASPECT_RATIO_IGNORE,
                OF_ALIGN_HORZ_CENTER,
                OF_ALIGN_VERT_CENTER);
    } else {
        scaleTo(targetRect,
                OF_ASPECT_RATIO_KEEP);
    }
}

//----------------------------------------------------------
void ofRectangle::scaleTo(const ofRectangle& targetRect,
                          ofAspectRatioMode subjectAspectRatioMode,
                          ofAlignHorz sharedHorzAnchor,
                          ofAlignVert sharedVertAnchor) {
    scaleTo(targetRect,
            subjectAspectRatioMode,
            sharedHorzAnchor,
            sharedVertAnchor,
            sharedHorzAnchor,
            sharedVertAnchor);
}

//----------------------------------------------------------
void ofRectangle::scaleTo(const ofRectangle& targetRect,
                                 ofAspectRatioMode aspectRatioMode,
                                 ofAlignHorz modelHorzAnchor,
                                 ofAlignVert modelVertAnchor,
                                 ofAlignHorz thisHorzAnchor,
                                 ofAlignVert thisVertAnchor) {

    float tw = targetRect.getWidth();    // target width
    float th = targetRect.getHeight();   // target height
    float sw = getWidth();   // subject width
    float sh = getHeight();  // subject height

    if(aspectRatioMode == OF_ASPECT_RATIO_KEEP_BY_EXPANDING ||
       aspectRatioMode == OF_ASPECT_RATIO_KEEP) {
        if(fabs(sw) >= FLT_EPSILON || fabs(sh) >= FLT_EPSILON) {
            float wRatio = fabs(tw) / fabs(sw);
            float hRatio = fabs(th) / fabs(sh);
            if(aspectRatioMode == OF_ASPECT_RATIO_KEEP_BY_EXPANDING) {
                scale(MAX(wRatio,hRatio));
            } else if(aspectRatioMode == OF_ASPECT_RATIO_KEEP) {
                scale(MIN(wRatio,hRatio));
            }
        } else {
            ofLogWarning("ofRectangle") << "scaleTo(): no scaling applied to avoid divide by zero, rectangle has 0 width and/or height: " << sw << "x" << sh;
        }
    } else if(aspectRatioMode == OF_ASPECT_RATIO_IGNORE) {
        width  = tw;
        height = th;
    } else {
        ofLogWarning("ofRectangle") << "scaleTo(): unknown ofAspectRatioMode = " << aspectRatioMode << ", using OF_ASPECT_RATIO_IGNORE";
        width  = tw;
        height = th;
    }

    // now align if anchors are not ignored.
    alignTo(targetRect,
            modelHorzAnchor,
            modelVertAnchor,
            thisHorzAnchor,
            thisVertAnchor);

}

//----------------------------------------------------------
void ofRectangle::alignToHorz(const float& targetX,
                              ofAlignHorz thisHorzAnchor) {

    if(thisHorzAnchor != OF_ALIGN_HORZ_IGNORE) {
        translateX(targetX - getHorzAnchor(thisHorzAnchor));
    } else {
        ofLogVerbose("ofRectangle") << "alignToHorz(): thisHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied";
    }
}

//----------------------------------------------------------
void ofRectangle::alignToHorz(const ofRectangle& targetRect,
                              ofAlignHorz sharedAnchor) {

    alignToHorz(targetRect, sharedAnchor, sharedAnchor);
}

//----------------------------------------------------------
void ofRectangle::alignToHorz(const ofRectangle& targetRect,
                              ofAlignHorz targetHorzAnchor,
                              ofAlignHorz thisHorzAnchor) {

    if(targetHorzAnchor != OF_ALIGN_HORZ_IGNORE &&
       thisHorzAnchor   != OF_ALIGN_HORZ_IGNORE) {
        alignToHorz(targetRect.getHorzAnchor(targetHorzAnchor),thisHorzAnchor);
    } else {
        if(targetHorzAnchor == OF_ALIGN_HORZ_IGNORE) {
            ofLogVerbose("ofRectangle") << "alignToHorz(): targetHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied";
        } else {
            ofLogVerbose("ofRectangle") << "alignToHorz(): thisHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied";
        }
    }

}

//----------------------------------------------------------
void ofRectangle::alignToVert(const float& targetY,
                              ofAlignVert thisVertAnchor) {

    if(thisVertAnchor != OF_ALIGN_VERT_IGNORE) {
        translateY(targetY - getVertAnchor(thisVertAnchor));
    } else {
        ofLogVerbose("ofRectangle") << "alignToVert(): thisVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied";
    }
}

//----------------------------------------------------------
void ofRectangle::alignToVert(const ofRectangle& targetRect,
                              ofAlignVert sharedAnchor) {

    alignToVert(targetRect,sharedAnchor,sharedAnchor);
}

//----------------------------------------------------------
void ofRectangle::alignToVert(const ofRectangle& targetRect,
                              ofAlignVert targetVertAnchor,
                              ofAlignVert thisVertAnchor) {

    if(targetVertAnchor != OF_ALIGN_VERT_IGNORE &&
       thisVertAnchor   != OF_ALIGN_VERT_IGNORE) {
        alignToVert(targetRect.getVertAnchor(targetVertAnchor),thisVertAnchor);
    } else {
        if(targetVertAnchor == OF_ALIGN_VERT_IGNORE) {
            ofLogVerbose("ofRectangle") << "alignToVert(): targetVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied";
        } else {
            ofLogVerbose("ofRectangle") << "alignToVert(): thisVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied";
        }

    }
}

//----------------------------------------------------------
void ofRectangle::alignTo(const ofPoint& targetPoint,
                          ofAlignHorz thisHorzAnchor,
                          ofAlignVert thisVertAnchor) {

    alignToHorz(targetPoint.x, thisHorzAnchor);
    alignToVert(targetPoint.y, thisVertAnchor);
}


//----------------------------------------------------------
void ofRectangle::alignTo(const ofRectangle& targetRect,
                          ofAlignHorz sharedHorzAnchor,
                          ofAlignVert sharedVertAnchor) {
    alignTo(targetRect,
            sharedHorzAnchor,
            sharedVertAnchor,
            sharedHorzAnchor,
            sharedVertAnchor);
}

//----------------------------------------------------------
void ofRectangle::alignTo(const ofRectangle& targetRect,
                                 ofAlignHorz targetHorzAnchor,
                                 ofAlignVert targetVertAnchor,
                                 ofAlignHorz thisHorzAnchor,
                                 ofAlignVert thisVertAnchor) {

    alignToHorz(targetRect,targetHorzAnchor,thisHorzAnchor);
    alignToVert(targetRect,targetVertAnchor,thisVertAnchor);
}

//----------------------------------------------------------
bool ofRectangle::inside(float px, float py) const {
	return inside(ofPoint(px,py));
}

//----------------------------------------------------------
bool ofRectangle::inside(const ofPoint& p) const {
    return p.x > getMinX() && p.y > getMinY() &&
           p.x < getMaxX() && p.y < getMaxY();
}

//----------------------------------------------------------
bool ofRectangle::inside(const ofRectangle& rect) const {
    return inside(rect.getMinX(),rect.getMinY()) &&
           inside(rect.getMaxX(),rect.getMaxY());
}

//----------------------------------------------------------
bool ofRectangle::inside(const ofPoint& p0, const ofPoint& p1) const {
    // check to see if a line segment is inside the rectangle
    return inside(p0) && inside(p1);
}

//----------------------------------------------------------
bool ofRectangle::intersects(const ofRectangle& rect) const {
    return (getMinX() < rect.getMaxX() && getMaxX() > rect.getMinX() &&
            getMinY() < rect.getMaxY() && getMaxY() > rect.getMinY());
}

//----------------------------------------------------------
bool ofRectangle::intersects(const ofPoint& p0, const ofPoint& p1) const {
    // check for a line intersection
    ofPoint p;

    ofPoint topLeft     = getTopLeft();
    ofPoint topRight    = getTopRight();
    ofPoint bottomRight = getBottomRight();
    ofPoint bottomLeft  = getBottomLeft();

    return inside(p0) || // check end inside
           inside(p1) || // check end inside
           ofLineSegmentIntersection(p0, p1, topLeft,     topRight,    p) || // cross top
           ofLineSegmentIntersection(p0, p1, topRight,    bottomRight, p) || // cross right
           ofLineSegmentIntersection(p0, p1, bottomRight, bottomLeft,  p) || // cross bottom
           ofLineSegmentIntersection(p0, p1, bottomLeft,  topLeft,     p);   // cross left
}

//----------------------------------------------------------
void ofRectangle::growToInclude(float px, float py) {
    growToInclude(ofPoint(px,py));
}

//----------------------------------------------------------
void ofRectangle::growToInclude(const ofPoint& p) {
    float x0 = MIN(getMinX(),p.x);
    float x1 = MAX(getMaxX(),p.x);
    float y0 = MIN(getMinY(),p.y);
    float y1 = MAX(getMaxY(),p.y);
    float w = x1 - x0;
    float h = y1 - y0;
    set(x0,y0,w,h);
}

//----------------------------------------------------------
void ofRectangle::growToInclude(const ofRectangle& rect) {
    float x0 = MIN(getMinX(),rect.getMinX());
    float x1 = MAX(getMaxX(),rect.getMaxX());
    float y0 = MIN(getMinY(),rect.getMinY());
    float y1 = MAX(getMaxY(),rect.getMaxY());
    float w = x1 - x0;
    float h = y1 - y0;
    set(x0,y0,w,h);
}

//----------------------------------------------------------
void ofRectangle::growToInclude(const ofPoint& p0, const ofPoint& p1) {
    growToInclude(p0);
    growToInclude(p1);
}

//----------------------------------------------------------
ofRectangle ofRectangle::getIntersection(const ofRectangle& rect) const {

    float x0 = MAX(getMinX(),rect.getMinX());
    float x1 = MIN(getMaxX(),rect.getMaxX());

    float w = x1 - x0;
    if(w < 0.0f) return ofRectangle(0,0,0,0); // short circuit if needed

    float y0 = MAX(getMinY(),rect.getMinY());
    float y1 = MIN(getMaxY(),rect.getMaxY());

    float h = y1 - y0;
    if(h < 0.0f) return ofRectangle(0,0,0,0);  // short circuit if needed

    return ofRectangle(x0,y0,w,h);
}

//----------------------------------------------------------
ofRectangle ofRectangle::getUnion(const ofRectangle& rect) const {
    ofRectangle united = *this;
    united.growToInclude(rect);
    return united;
}

//----------------------------------------------------------
void ofRectangle::standardize() {
    if(width < 0.0f) {
        x += width;
        width = -1.0f * width;
    }

    if(height < 0.0f) {
        y += height;
        height = -1.0f * height;
    }
}

//----------------------------------------------------------
ofRectangle ofRectangle::getStandardized() const {
    if(isStandardized()) {
        return *this;
    } else {
        ofRectangle canRect(*this); // copy it
        canRect.standardize();
        return canRect;
    }
}

//----------------------------------------------------------
bool ofRectangle::isStandardized() const {
    return width >= 0.0f && height >= 0.0f;
}

//----------------------------------------------------------
float ofRectangle::getArea() const {
    return fabs(width) * fabs(height);
}

//----------------------------------------------------------
float ofRectangle::getPerimeter() const {
    return 2.0f * fabs(width) + 2.0f * fabs(height);
}

//----------------------------------------------------------
float ofRectangle::getAspectRatio() const {
    return fabs(width) / fabs(height);
}

//----------------------------------------------------------
bool ofRectangle::isEmpty() const {
    return width == 0.0f && height == 0.0f;
}

//----------------------------------------------------------
ofPoint ofRectangle::getMin() const {
    return ofPoint(getMinX(),getMinY());
}

//----------------------------------------------------------
ofPoint ofRectangle::getMax() const {
    return ofPoint(getMaxX(),getMaxY());
}

//----------------------------------------------------------
float ofRectangle::getMinX() const {
    return MIN(x, x + width);  // - width
}

//----------------------------------------------------------
float ofRectangle::getMaxX() const {
    return MAX(x, x + width);  // - width
}

//----------------------------------------------------------
float ofRectangle::getMinY() const{
    return MIN(y, y + height);  // - height
}

//----------------------------------------------------------
float ofRectangle::getMaxY() const {
    return MAX(y, y + height);  // - height
}

//----------------------------------------------------------
float ofRectangle::getLeft() const {
    return getMinX();
}

//----------------------------------------------------------
float ofRectangle::getRight() const {
    return getMaxX();
}

//----------------------------------------------------------
float ofRectangle::getTop() const {
    return getMinY();
}

//----------------------------------------------------------
float ofRectangle::getBottom() const {
    return getMaxY();
}

//----------------------------------------------------------
ofPoint ofRectangle::getTopLeft() const {
    return getMin();
}

//----------------------------------------------------------
ofPoint ofRectangle::getTopRight() const {
    return ofPoint(getRight(),getTop());
}

//----------------------------------------------------------
ofPoint ofRectangle::getBottomLeft() const {
    return ofPoint(getLeft(),getBottom());
}

//----------------------------------------------------------
ofPoint ofRectangle::getBottomRight() const {
    return getMax();
}

//----------------------------------------------------------
float ofRectangle::getHorzAnchor(ofAlignHorz anchor) const {
    switch (anchor) {
        case OF_ALIGN_HORZ_IGNORE:
            ofLogError("ofRectangle") << "getHorzAnchor(): unable to get anchor for OF_ALIGN_HORZ_IGNORE, returning 0";
            return 0.0f;
        case OF_ALIGN_HORZ_LEFT:
            return getLeft();
        case OF_ALIGN_HORZ_RIGHT:
            return getRight();
        case OF_ALIGN_HORZ_CENTER:
            return getCenter().x;
        default:
            ofLogError("ofRectangle") << "getHorzAnchor(): unknown ofAlignHorz = " << anchor << ", returning 0.0";
            return 0.0f;
    }
}

//----------------------------------------------------------
float ofRectangle::getVertAnchor(ofAlignVert anchor) const {
    switch (anchor) {
        case OF_ALIGN_VERT_IGNORE:
            ofLogError("ofRectangle") << "getVertAnchor(): unable to get anchor for OF_ALIGN_VERT_IGNORE, returning 0.0";
            return 0.0f;
        case OF_ALIGN_VERT_TOP:
            return getTop();
        case OF_ALIGN_VERT_BOTTOM:
            return getBottom();
        case OF_ALIGN_VERT_CENTER:
            return getCenter().y;
        default:
            ofLogError("ofRectangle") << "getVertAnchor(): unknown ofAlignVert = " << anchor << ", returning 0.0";
            return 0.0f;
    }
}

//----------------------------------------------------------
bool ofRectangle::operator != (const ofRectangle& rect) const {
	return (x != rect.x) || (y != rect.y) || (width != rect.width) || (height != rect.height);
}

//----------------------------------------------------------
ofPoint ofRectangle::getPosition() const {
    return position;
}

//----------------------------------------------------------
ofPoint& ofRectangle::getPositionRef() {
    return position;
}

//----------------------------------------------------------
ofPoint ofRectangle::getCenter() const {
	return ofPoint(x + width * 0.5f, y + height * 0.5f, 0);
}

//----------------------------------------------------------
float ofRectangle::getX() const {
    return x;
}

//----------------------------------------------------------
float ofRectangle::getY() const {
    return y;
}

//----------------------------------------------------------
float ofRectangle::getWidth() const {
    return width;
}

//----------------------------------------------------------
float ofRectangle::getHeight() const {
    return height;
}

//----------------------------------------------------------
ofRectangle& ofRectangle::operator = (const ofRectangle& rect) {
    set(rect);
	return *this;
}

//----------------------------------------------------------
ofRectangle ofRectangle::operator + (const ofPoint & point){
	ofRectangle rect=*this;
	rect.x += point.x;
	rect.y += point.y;
	return rect;
}

//----------------------------------------------------------
ofRectangle ofRectangle::operator - (const ofPoint & point){
	ofRectangle rect=*this;
	rect.x -= point.x;
	rect.y -= point.y;
	return rect;
}

//----------------------------------------------------------
bool ofRectangle::operator == (const ofRectangle& rect) const {
	return (x == rect.x) && (y == rect.y) && (width == rect.width) && (height == rect.height);
}

//----------------------------------------------------------
bool ofRectangle::isZero() const{
	return (x == 0) && (y == 0) && (width == 0) && (height == 0);
}

//----------------------------------------------------------
ostream& operator<<(ostream& os, const ofRectangle& rect){
	os << rect.position << ", " << rect.width << ", " << rect.height;
	return os;
}

//----------------------------------------------------------
istream& operator>>(istream& is, ofRectangle& rect){
	is >> rect.position;
	is.ignore(2);
	is >> rect.width;
	is.ignore(2);
	is >> rect.height;
	return is;
}
