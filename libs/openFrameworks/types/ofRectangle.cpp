#include "ofRectangle.h"
#include "ofMath.h"
#include "ofLog.h"
#include "ofVectorMath.h"

using std::ostream;
using std::istream;
//----------------------------------------------------------
ofRectangle::ofRectangle() : x(position.x), y(position.y) {
    set(0,0,0,0);
}

//----------------------------------------------------------
ofRectangle::~ofRectangle(){}

//----------------------------------------------------------
ofRectangle::ofRectangle(float px, float py, float w, float h) : x(position.x), y(position.y) {
	set(px,py,w,h);
}

//----------------------------------------------------------
ofRectangle::ofRectangle(const glm::vec3& p, float w, float h) : x(position.x), y(position.y) {
    set(p,w,h);
}

//----------------------------------------------------------
ofRectangle::ofRectangle(const glm::vec2& p, float w, float h) : x(position.x), y(position.y) {
	set(p,w,h);
}

//----------------------------------------------------------
ofRectangle::ofRectangle(const ofRectangle& rect) : x(position.x), y(position.y) {
    set(rect);
}

//----------------------------------------------------------
ofRectangle::ofRectangle(const glm::vec3& p0, const glm::vec3& p1) : x(position.x), y(position.y) {
    set(p0,p1);
}

//----------------------------------------------------------
ofRectangle::ofRectangle(const glm::vec2& p0, const glm::vec2& p1) : x(position.x), y(position.y) {
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
void ofRectangle::set(const glm::vec3& p, float w, float h) {
	set(glm::vec2(p), w, h);
}

//----------------------------------------------------------
void ofRectangle::set(const glm::vec2& p, float w, float h) {
	set(p.x, p.y, w, h);
}

//----------------------------------------------------------
void ofRectangle::set(const ofRectangle& rect){
    set(rect.x, rect.y, rect.width, rect.height);
}

//----------------------------------------------------------
void ofRectangle::set(const glm::vec3& p0, const glm::vec3& p1) {
	set(glm::vec2(p0), glm::vec2(p1));
}

//----------------------------------------------------------
void ofRectangle::set(const glm::vec2& p0, const glm::vec2& p1) {
	float x0,y0,x1,y1;

	x0 = std::min(p0.x, p1.x);
	x1 = std::max(p0.x, p1.x);
	y0 = std::min(p0.y, p1.y);
	y1 = std::max(p0.y, p1.y);

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
void ofRectangle::setPosition(const glm::vec3& p) {
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
void ofRectangle::setFromCenter(const glm::vec3& p, float w, float h) {
	setFromCenter(glm::vec2(p),w,h);
}

//----------------------------------------------------------
void ofRectangle::setFromCenter(const glm::vec2& p, float w, float h) {
	setFromCenter(p.x, p.y, w, h);
}

//----------------------------------------------------------
void ofRectangle::translate(float dx, float dy) {
    translateX(dx);
    translateY(dy);
}

//----------------------------------------------------------
void ofRectangle::translate(const glm::vec3& dp) {
	translate(glm::vec2(dp));
}

//----------------------------------------------------------
void ofRectangle::translate(const glm::vec2& dp) {
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
void ofRectangle::scale(const glm::vec3& s) {
	scale(glm::vec2(s));
}

//----------------------------------------------------------
void ofRectangle::scale(const glm::vec2& s) {
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
	scaleFromCenter({sX,sY,0.f});
}

//----------------------------------------------------------
void ofRectangle::scaleFromCenter(const glm::vec3& s) {
	scaleFromCenter(glm::vec2(s));
}

//----------------------------------------------------------
void ofRectangle::scaleFromCenter(const glm::vec2& s) {
	if(ofIsFloatEqual(s.x, 1.0f) && ofIsFloatEqual(s.y, 1.0f)) return; // nothing to do

	float newWidth  = width  * s.x;
	float newHeight = height * s.y;

	auto center = getCenter();

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
        if(fabs(sw) >= std::numeric_limits<float>::epsilon() || fabs(sh) >= FLT_EPSILON) {
            float wRatio = fabs(tw) / fabs(sw);
            float hRatio = fabs(th) / fabs(sh);
            if(aspectRatioMode == OF_ASPECT_RATIO_KEEP_BY_EXPANDING) {
				scale(std::max(wRatio,hRatio));
            } else if(aspectRatioMode == OF_ASPECT_RATIO_KEEP) {
				scale(std::min(wRatio,hRatio));
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
void ofRectangle::alignTo(const glm::vec3& targetPoint,
						  ofAlignHorz thisHorzAnchor,
						  ofAlignVert thisVertAnchor) {

	alignTo(glm::vec2(targetPoint), thisHorzAnchor, thisVertAnchor);
}

//----------------------------------------------------------
void ofRectangle::alignTo(const glm::vec2& targetPoint,
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
	return inside({px,py});
}

//----------------------------------------------------------
bool ofRectangle::inside(const glm::vec3& p) const {
	return inside(glm::vec2(p));
}

//----------------------------------------------------------
bool ofRectangle::inside(const glm::vec2& p) const {
	return p.x > getMinX() && p.y > getMinY() &&
		   p.x < getMaxX() && p.y < getMaxY();
}

//----------------------------------------------------------
bool ofRectangle::inside(const ofRectangle& rect) const {
    return inside(rect.getMinX(),rect.getMinY()) &&
           inside(rect.getMaxX(),rect.getMaxY());
}

//----------------------------------------------------------
bool ofRectangle::inside(const glm::vec3& p0, const glm::vec3& p1) const {
    // check to see if a line segment is inside the rectangle
	return inside(glm::vec2(p0), glm::vec2(p1));
}

//----------------------------------------------------------
bool ofRectangle::inside(const glm::vec2& p0, const glm::vec2& p1) const {
	// check to see if a line segment is inside the rectangle
	return inside(p0) && inside(p1);
}

//----------------------------------------------------------
bool ofRectangle::intersects(const ofRectangle& rect) const {
    return (getMinX() < rect.getMaxX() && getMaxX() > rect.getMinX() &&
            getMinY() < rect.getMaxY() && getMaxY() > rect.getMinY());
}

//----------------------------------------------------------
bool ofRectangle::intersects(const glm::vec3& p0, const glm::vec3& p1) const {
	return intersects(glm::vec2(p0), glm::vec2(p1));
}

//----------------------------------------------------------
bool ofRectangle::intersects(const glm::vec2& p0, const glm::vec2& p1) const {
	// check for a line intersection
	glm::vec2 p;

	glm::vec2 topLeft(getTopLeft());
	glm::vec2 topRight(getTopRight());
	glm::vec2 bottomRight(getBottomRight());
	glm::vec2 bottomLeft(getBottomLeft());

	return inside(p0) || // check end inside
		   inside(p1) || // check end inside
		   ofLineSegmentIntersection(p0, p1, topLeft,     topRight,    p) || // cross top
		   ofLineSegmentIntersection(p0, p1, topRight,    bottomRight, p) || // cross right
		   ofLineSegmentIntersection(p0, p1, bottomRight, bottomLeft,  p) || // cross bottom
		   ofLineSegmentIntersection(p0, p1, bottomLeft,  topLeft,     p);   // cross left
}

//----------------------------------------------------------
void ofRectangle::growToInclude(float px, float py) {
	growToInclude({px,py});
}

//----------------------------------------------------------
void ofRectangle::growToInclude(const glm::vec3& p) {
	growToInclude(glm::vec2(p));
}

//----------------------------------------------------------
void ofRectangle::growToInclude(const glm::vec2& p) {
	float x0 = std::min(getMinX(),p.x);
	float x1 = std::max(getMaxX(),p.x);
	float y0 = std::min(getMinY(),p.y);
	float y1 = std::max(getMaxY(),p.y);
	float w = x1 - x0;
	float h = y1 - y0;
	set(x0,y0,w,h);
}

//----------------------------------------------------------
void ofRectangle::growToInclude(const ofRectangle& rect) {
	float x0 = std::min(getMinX(),rect.getMinX());
	float x1 = std::max(getMaxX(),rect.getMaxX());
	float y0 = std::min(getMinY(),rect.getMinY());
	float y1 = std::max(getMaxY(),rect.getMaxY());
    float w = x1 - x0;
    float h = y1 - y0;
    set(x0,y0,w,h);
}

//----------------------------------------------------------
void ofRectangle::growToInclude(const glm::vec3& p0, const glm::vec3& p1) {
	growToInclude(glm::vec2(p0), glm::vec2(p1));
}

//----------------------------------------------------------
void ofRectangle::growToInclude(const glm::vec2& p0, const glm::vec2& p1) {
	growToInclude(p0);
	growToInclude(p1);
}

//----------------------------------------------------------
ofRectangle ofRectangle::getIntersection(const ofRectangle& rect) const {

	float x0 = std::max(getMinX(),rect.getMinX());
	float x1 = std::min(getMaxX(),rect.getMaxX());

    float w = x1 - x0;
    if(w < 0.0f) return ofRectangle(0,0,0,0); // short circuit if needed

	float y0 = std::max(getMinY(),rect.getMinY());
	float y1 = std::min(getMaxY(),rect.getMaxY());

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
    return ofIsFloatEqual(width, 0.0f) && ofIsFloatEqual(height, 0.0f);
}

//----------------------------------------------------------
glm::vec3 ofRectangle::getMin() const {
	return {getMinX(),getMinY(),0.f};
}

//----------------------------------------------------------
glm::vec3 ofRectangle::getMax() const {
	return {getMaxX(),getMaxY(),0.f};
}

//----------------------------------------------------------
float ofRectangle::getMinX() const {
	return std::min(x, x + width);  // - width
}

//----------------------------------------------------------
float ofRectangle::getMaxX() const {
	return std::max(x, x + width);  // - width
}

//----------------------------------------------------------
float ofRectangle::getMinY() const{
	return std::min(y, y + height);  // - height
}

//----------------------------------------------------------
float ofRectangle::getMaxY() const {
	return std::max(y, y + height);  // - height
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
glm::vec3 ofRectangle::getTopLeft() const {
    return getMin();
}

//----------------------------------------------------------
glm::vec3 ofRectangle::getTopRight() const {
	return {getRight(), getTop(), 0.f};
}

//----------------------------------------------------------
glm::vec3 ofRectangle::getBottomLeft() const {
	return glm::vec3(getLeft(), getBottom(), 0.f);
}

//----------------------------------------------------------
glm::vec3 ofRectangle::getBottomRight() const {
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
const glm::vec3& ofRectangle::getPosition() const {
	return position;
}

//----------------------------------------------------------
glm::vec3& ofRectangle::getPositionRef() {
	return position;
}

//----------------------------------------------------------
glm::vec3 ofRectangle::getCenter() const {
	return {x + width * 0.5f, y + height * 0.5f, 0.f};
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
glm::vec2 ofRectangle::map(const glm::vec2 & coeff) const {
    return glm::vec2(
        ofMap(coeff.x, 0.0f, 1.0f, getMinX(), getMaxX(), false), 
        ofMap(coeff.y, 0.0f, 1.0f, getMinY(), getMaxY(), false)
        );
}

//----------------------------------------------------------

ofRectangle ofRectangle::map(const ofRectangle & coeff) const {
    return ofRectangle(
       map(glm::vec2(coeff.getMinX(), coeff.getMinY())),
       map(glm::vec2(coeff.getMaxX(), coeff.getMaxY()))
       );
}

glm::vec2 ofRectangle::mapClamp(const glm::vec2 & coeff) const {
    return glm::vec2(
        ofMap(coeff.x, 0.0f, 1.0f, getMinX(), getMaxX(), true), 
        ofMap(coeff.y, 0.0f, 1.0f, getMinY(), getMaxY(), true)
        );
}

ofRectangle ofRectangle::mapClamp(const ofRectangle & coeff) const {
    return ofRectangle(
       mapClamp(glm::vec2(coeff.getMinX(), coeff.getMinY())),
       mapClamp(glm::vec2(coeff.getMaxX(), coeff.getMaxY()))
       );
}
    

//----------------------------------------------------------
ofRectangle& ofRectangle::operator = (const ofRectangle& rect) {
    set(rect);
	return *this;
}

//----------------------------------------------------------
ofRectangle ofRectangle::operator + (const glm::vec3 & point){
	return operator+(glm::vec2(point));
}

//----------------------------------------------------------
ofRectangle ofRectangle::operator + (const glm::vec2 & point){
	ofRectangle rect=*this;
	rect.x += point.x;
	rect.y += point.y;
	return rect;
}

//----------------------------------------------------------
ofRectangle ofRectangle::operator - (const glm::vec3 & point){
	return operator-(glm::vec2(point));
}

//----------------------------------------------------------
ofRectangle ofRectangle::operator - (const glm::vec2 & point){
	ofRectangle rect=*this;
	rect.x -= point.x;
	rect.y -= point.y;
	return rect;
}

//----------------------------------------------------------
bool ofRectangle::operator == (const ofRectangle& rect) const {
	return ofIsFloatEqual(x, rect.x) && ofIsFloatEqual(y, rect.y) && ofIsFloatEqual(width, rect.width) && ofIsFloatEqual(height, rect.height);
}

//----------------------------------------------------------
bool ofRectangle::isZero() const{
	return ofIsFloatEqual(x, 0.0f) && ofIsFloatEqual(y, 0.0f) && ofIsFloatEqual(width, 0.0f) && ofIsFloatEqual(height, 0.0f);
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
