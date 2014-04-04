#include <cfloat>
#include "ofBox.h"

//----------------------------------------------------------
ofBox::ofBox()
		: x(position.x), y(position.y), z(position.z),
		width(size.x), height(size.y), depth(size.z){
    set(0,0,0,0,0,0);
}

//----------------------------------------------------------
ofBox::~ ofBox(){}

//----------------------------------------------------------
ofBox::ofBox(float px, float py, float pz, float w, float h, float d)
			: x(position.x), y(position.y), z(position.z),
			width(size.x), height(size.y), depth(size.z){
	set(px,py,pz,w,h,d);
}

//----------------------------------------------------------
ofBox::ofBox(const ofPoint& p, float w, float h, float d)
			: x(position.x), y(position.y), z(position.z),
			width(size.x), height(size.y), depth(size.z){
    set(p,w,h,d);
}

//----------------------------------------------------------
ofBox::ofBox(const ofBox& box)
			: x(position.x), y(position.y), z(position.z),
			width(size.x), height(size.y), depth(size.z){
    set(box);
}

//----------------------------------------------------------
ofBox::ofBox(const ofPoint& p0, const ofPoint& p1)
			: x(position.x), y(position.y), z(position.z),
			width(size.x), height(size.y), depth(size.z){
    set(p0,p1);
}

//----------------------------------------------------------
void ofBox::set(float px, float py, float pz, float w, float h, float d) {
	x		= px;
	y		= py;
	z		= pz;
	width	= w;
	height	= h;
	depth	= d;
}

//----------------------------------------------------------
void ofBox::set(const ofPoint& p, float w, float h, float d) {
    set(p.x, p.y, p.z, w, h, d);
}

//----------------------------------------------------------
void ofBox::set(const ofBox& box){
    set(box.x, box.y, box.z, box.width, box.height, box.depth);
}

//----------------------------------------------------------
void ofBox::set(const ofPoint& p0, const ofPoint& p1) {
    float x0,y0,x1,y1,z0,z1;
    
    x0 = MIN(p0.x, p1.x);
    x1 = MAX(p0.x, p1.x);
    y0 = MIN(p0.y, p1.y);
    y1 = MAX(p0.y, p1.y);
    z0 = MIN(p0.z, p1.z);
    z1 = MAX(p0.z, p1.z);
    
    float w = x1 - x0;
    float h = y1 - y0;
    float d = z1 - z0;
	
    set(x0,y0,z0,w,h,d);
}

//----------------------------------------------------------
void ofBox::setX(float px) {
    x = px;
}

//----------------------------------------------------------
void ofBox::setY(float py) {
    y = py;
}

//----------------------------------------------------------
void ofBox::setZ(float pz) {
    z = pz;
}

//----------------------------------------------------------
void ofBox::setWidth(float w) {
    width = w;
}

//----------------------------------------------------------
void ofBox::setHeight(float h) {
    height = h;
}

//----------------------------------------------------------
void ofBox::setDepth(float d) {
    depth = d;
}

//----------------------------------------------------------
void ofBox::setPosition(float px, float py, float pz) {
    position.x = px;
    position.y = py;
    position.z = pz;
}

//----------------------------------------------------------
void ofBox::setPosition(const ofPoint& p) {
    position = p;
}

//----------------------------------------------------------
void ofBox::setSize(float sx, float sy, float sz) {
    size.x = sx;
    size.y = sy;
    size.z = sz;
}

//----------------------------------------------------------
void ofBox::setSize(const ofPoint& s) {
    size = s;
}

//----------------------------------------------------------
void ofBox::setFromCenter(float px, float py, float pz, float w, float h, float d) {
    set(px - w*0.5f, py - h*0.5f, pz - d*0.5f, w, h,d);
}

//----------------------------------------------------------
void ofBox::setFromCenter(const ofPoint& p, float w, float h, float d) {
    setFromCenter(p.x, p.y, p.z, w, h, d);
}

//----------------------------------------------------------
void ofBox::translate(float dx, float dy, float dz) {
    translateX(dx);
    translateY(dy);
    translateZ(dz);
}

//----------------------------------------------------------
void ofBox::translate(const ofPoint& dp) {
    translateX(dp.x);
    translateY(dp.y);
    translateZ(dp.z);
}

//----------------------------------------------------------
void ofBox::translateX(float dx) {
    x += dx;
}

//----------------------------------------------------------
void ofBox::translateY(float dy) {
    y += dy;
}

//----------------------------------------------------------
void ofBox::translateZ(float dz) {
    z += dz;
}

//----------------------------------------------------------
void ofBox::scale(float s) {
    scaleWidth(s);
    scaleHeight(s);
    scaleDepth(s);
}

//----------------------------------------------------------
void ofBox::scale(float sX, float sY, float sZ) {
    scaleWidth(sX);
    scaleHeight(sY);
    scaleDepth(sZ);
}

//----------------------------------------------------------
void ofBox::scale(const ofPoint& s) {
    scaleWidth(s.x);
    scaleHeight(s.y);
}

//----------------------------------------------------------
void ofBox::scaleWidth(float  sX) {
    width  *= sX;
}
//----------------------------------------------------------
void ofBox::scaleHeight(float sY) {
    height *= sY;
}
//----------------------------------------------------------
void ofBox::scaleDepth(float sZ) {
    depth *= sZ;
}

//----------------------------------------------------------
void ofBox::scaleFromCenter(float s) {
    scaleFromCenter(s,s,s);
}

//----------------------------------------------------------
void ofBox::scaleFromCenter(float sX, float sY, float sZ) {
    scaleFromCenter(ofPoint(sX,sY,sZ));
}

//----------------------------------------------------------
void ofBox::scaleFromCenter(const ofPoint& s) {
    if(s.x == 1.0f && s.y == 1.0f && s.z == 1.0f) return; // nothing to do
    
    float newWidth  = width  * s.x;
    float newHeight = height * s.y;
    float newDepth = depth * s.z;
	
    ofPoint center = getCenter();
    
    x = center.x - newWidth  / 2.0f;
    y = center.y - newHeight / 2.0f;
    z = center.z - newDepth / 2.0f;
    
    width  = newWidth;
    height = newHeight;
    depth = newDepth;
}

/* //TODO
//----------------------------------------------------------
void ofBox::scaleTo(const ofBox& targetRect,
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
void ofBox::scaleTo(const ofBox& targetRect,
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
void ofBox::scaleTo(const ofBox& targetRect,
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
            ofLogWarning("ofBox") << "scaleTo(): no scaling applied to avoid divide by zero, box has 0 width and/or height: " << sw << "x" << sh;
        }
    } else if(aspectRatioMode == OF_ASPECT_RATIO_IGNORE) {
        width  = tw;
        height = th;
    } else {
        ofLogWarning("ofBox") << "scaleTo(): unknown ofAspectRatioMode = " << aspectRatioMode << ", using OF_ASPECT_RATIO_IGNORE";
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
void ofBox::alignToHorz(const float& targetX,
                              ofAlignHorz thisHorzAnchor) {
	
    if(thisHorzAnchor != OF_ALIGN_HORZ_IGNORE) {
        translateX(targetX - getHorzAnchor(thisHorzAnchor));
    } else {
        ofLogVerbose("ofBox") << "alignToHorz(): thisHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied";
    }
}

//----------------------------------------------------------
void ofBox::alignToHorz(const ofBox& targetRect,
                              ofAlignHorz sharedAnchor) {
    
    alignToHorz(targetRect, sharedAnchor, sharedAnchor);
}

//----------------------------------------------------------
void ofBox::alignToHorz(const ofBox& targetRect,
                              ofAlignHorz targetHorzAnchor,
                              ofAlignHorz thisHorzAnchor) {
    
    if(targetHorzAnchor != OF_ALIGN_HORZ_IGNORE &&
       thisHorzAnchor   != OF_ALIGN_HORZ_IGNORE) {
        alignToHorz(targetRect.getHorzAnchor(targetHorzAnchor),thisHorzAnchor);
    } else {
        if(targetHorzAnchor == OF_ALIGN_HORZ_IGNORE) {
            ofLogVerbose("ofBox") << "alignToHorz(): targetHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied";
        } else {
            ofLogVerbose("ofBox") << "alignToHorz(): thisHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied";
        }
    }
    
}

//----------------------------------------------------------
void ofBox::alignToVert(const float& targetY,
                              ofAlignVert thisVertAnchor) {
	
    if(thisVertAnchor != OF_ALIGN_VERT_IGNORE) {
        translateY(targetY - getVertAnchor(thisVertAnchor));
    } else {
        ofLogVerbose("ofBox") << "alignToVert(): thisVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied";
    }
}

//----------------------------------------------------------
void ofBox::alignToVert(const ofBox& targetRect,
                              ofAlignVert sharedAnchor) {
    
    alignToVert(targetRect,sharedAnchor,sharedAnchor);
}

//----------------------------------------------------------
void ofBox::alignToVert(const ofBox& targetRect,
                              ofAlignVert targetVertAnchor,
                              ofAlignVert thisVertAnchor) {
	
    if(targetVertAnchor != OF_ALIGN_VERT_IGNORE &&
       thisVertAnchor   != OF_ALIGN_VERT_IGNORE) {
        alignToVert(targetRect.getVertAnchor(targetVertAnchor),thisVertAnchor);
    } else {
        if(targetVertAnchor == OF_ALIGN_VERT_IGNORE) {
            ofLogVerbose("ofBox") << "alignToVert(): targetVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied";
        } else {
            ofLogVerbose("ofBox") << "alignToVert(): thisVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied";
        }
        
    }
}

//----------------------------------------------------------
void ofBox::alignTo(const ofPoint& targetPoint,
                          ofAlignHorz thisHorzAnchor,
                          ofAlignVert thisVertAnchor) {
	
    alignToHorz(targetPoint.x, thisHorzAnchor);
    alignToVert(targetPoint.y, thisVertAnchor);
}


//----------------------------------------------------------
void ofBox::alignTo(const ofBox& targetRect,
                          ofAlignHorz sharedHorzAnchor,
                          ofAlignVert sharedVertAnchor) {
    alignTo(targetRect,
            sharedHorzAnchor,
            sharedVertAnchor,
            sharedHorzAnchor,
            sharedVertAnchor);
}

//----------------------------------------------------------
void ofBox::alignTo(const ofBox& targetRect,
						  ofAlignHorz targetHorzAnchor,
						  ofAlignVert targetVertAnchor,
						  ofAlignHorz thisHorzAnchor,
						  ofAlignVert thisVertAnchor) {
	
    alignToHorz(targetRect,targetHorzAnchor,thisHorzAnchor);
    alignToVert(targetRect,targetVertAnchor,thisVertAnchor);
}
 */

//----------------------------------------------------------
bool ofBox::inside(float px, float py, float pz) const {
	return inside(ofPoint(px,py,pz));
}

//----------------------------------------------------------
bool ofBox::inside(const ofPoint& p) const {
    return	p.x > getMinX() && p.y > getMinY() && p.z > getMinZ() &&
			p.x < getMaxX() && p.y < getMaxY() && p.z < getMaxZ();
}

//----------------------------------------------------------
bool ofBox::inside(const ofBox& box) const {
    return	inside(box.getMinX(),box.getMinY(),box.getMinZ()) &&
			inside(box.getMaxX(),box.getMaxY(),box.getMaxZ());
}

//----------------------------------------------------------
bool ofBox::inside(const ofPoint& p0, const ofPoint& p1) const {
    // check to see if a line segment is inside the box
    return inside(p0) && inside(p1);
}

//----------------------------------------------------------
bool ofBox::intersects(const ofBox& box) const {
    return (getMinX() < box.getMaxX() && getMaxX() > box.getMinX() &&
            getMinY() < box.getMaxY() && getMaxY() > box.getMinY() &&
			getMinZ() < box.getMaxZ() && getMaxZ() > box.getMinZ());
}

/* TODO
//----------------------------------------------------------
bool ofBox::intersects(const ofPoint& p0, const ofPoint& p1) const {
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
*/

//----------------------------------------------------------
void ofBox::growToInclude(float px, float py, float pz) {
    growToInclude(ofPoint(px,py,pz));
}

//----------------------------------------------------------
void ofBox::growToInclude(const ofPoint& p) {
    float x0 = MIN(getMinX(),p.x);
    float x1 = MAX(getMaxX(),p.x);
    float y0 = MIN(getMinY(),p.y);
    float y1 = MAX(getMaxY(),p.y);
    float z0 = MIN(getMinY(),p.z);
    float z1 = MAX(getMaxY(),p.z);
    float w = x1 - x0;
    float h = y1 - y0;
    float d = z1 - z0;
    set(x0,y0,z0,w,h,d);
}

//----------------------------------------------------------
void ofBox::growToInclude(const ofBox& box) {
    float x0 = MIN(getMinX(),box.getMinX());
    float x1 = MAX(getMaxX(),box.getMaxX());
    float y0 = MIN(getMinY(),box.getMinY());
    float y1 = MAX(getMaxY(),box.getMaxY());
    float z0 = MIN(getMinZ(),box.getMinZ());
    float z1 = MAX(getMaxZ(),box.getMaxZ());
    float w = x1 - x0;
    float h = y1 - y0;
    float d = z1 - z0;
    set(x0,y0,z0,w,h,d);
}

//----------------------------------------------------------
void ofBox::growToInclude(const ofPoint& p0, const ofPoint& p1) {
    growToInclude(p0);
    growToInclude(p1);
}

//----------------------------------------------------------
ofBox ofBox::getIntersection(const ofBox& box) const {
	
    float x0 = MAX(getMinX(),box.getMinX());
    float x1 = MIN(getMaxX(),box.getMaxX());
    
    float w = x1 - x0;
    if(w < 0.0f) return ofBox(0,0,0,0,0,0); // short circuit if needed
    
    float y0 = MAX(getMinY(),box.getMinY());
    float y1 = MIN(getMaxY(),box.getMaxY());
    
    float h = y1 - y0;
    if(h < 0.0f) return ofBox(0,0,0,0,0,0);  // short circuit if needed

    float z0 = MAX(getMinZ(),box.getMinZ());
    float z1 = MIN(getMaxZ(),box.getMaxZ());
    
    float d = z1 - z0;
    if(d < 0.0f) return ofBox(0,0,0,0,0,0);  // short circuit if needed
    
    return ofBox(x0,y0,z0,w,h,d);
}

//----------------------------------------------------------
ofBox ofBox::getUnion(const ofBox& box) const {
    ofBox united = *this;
    united.growToInclude(box);
    return united;
}

//----------------------------------------------------------
void ofBox::standardize() {
    if(width < 0.0f) {
        x += width;
        width = -1.0f * width;
    }
    
    if(height < 0.0f) {
        y += height;
        height = -1.0f * height;
    }

    if(depth < 0.0f) {
        z += depth;
        depth = -1.0f * depth;
    }

}

//----------------------------------------------------------
ofBox ofBox::getStandardized() const {
    if(isStandardized()) {
        return *this;
    } else {
        ofBox canRect(*this); // copy it
        canRect.standardize();
        return canRect;
    }
}

//----------------------------------------------------------
bool ofBox::isStandardized() const {
    return width >= 0.0f && height >= 0.0f && depth >= 0.0f;
}

//----------------------------------------------------------
float ofBox::getArea() const {
    return 2.0f * (fabs(width) * fabs(height) +
				   fabs(height) * fabs(depth) +
				   fabs(width) * fabs(depth));
}

//----------------------------------------------------------
float ofBox::getVolume() const {
    return fabs(width) * fabs(height) * fabs(depth);
}
/*
//----------------------------------------------------------
float ofBox::getAspectRatio() const {
    return fabs(width) / fabs(height);
}
*/
//----------------------------------------------------------
bool ofBox::isEmpty() const {
    return width == 0.0f && height == 0.0f && depth == 0.0f;
}

//----------------------------------------------------------
ofPoint ofBox::getMin() const {
    return ofPoint(getMinX(),getMinY(),getMinZ());
}

//----------------------------------------------------------
ofPoint ofBox::getMax() const {
    return ofPoint(getMaxX(),getMaxY(),getMaxZ());
}

//----------------------------------------------------------
float ofBox::getMinX() const {
    return MIN(x, x + width);  // - width
}
//----------------------------------------------------------
float ofBox::getMaxX() const {
    return MAX(x, x + width);  // - width
}
//----------------------------------------------------------
float ofBox::getMinY() const{
    return MIN(y, y + height);  // - height
}
//----------------------------------------------------------
float ofBox::getMaxY() const {
    return MAX(y, y + height);  // - height
}
//----------------------------------------------------------
float ofBox::getMinZ() const{
    return MIN(z, z + depth);  // - depth
}
//----------------------------------------------------------
float ofBox::getMaxZ() const {
    return MAX(z, z + depth);  // - depth
}

//----------------------------------------------------------
float ofBox::getLeft() const {
    return getMinX();
}
//----------------------------------------------------------
float ofBox::getRight() const {
    return getMaxX();
}
//----------------------------------------------------------
float ofBox::getTop() const {
    return getMinY();
}
//----------------------------------------------------------
float ofBox::getBottom() const {
    return getMaxY();
}
//----------------------------------------------------------
float ofBox::getFront() const {
    return getMinZ();
}
//----------------------------------------------------------
float ofBox::getBack() const {
    return getMaxZ();
}
/*
//----------------------------------------------------------
ofPoint ofBox::getTopLeft() const {
    return getMin();
}
//----------------------------------------------------------
ofPoint ofBox::getTopRight() const {
    return ofPoint(getRight(),getTop());
}
//----------------------------------------------------------
ofPoint ofBox::getBottomLeft() const {
    return ofPoint(getLeft(),getBottom());
}
//----------------------------------------------------------
ofPoint ofBox::getBottomRight() const {
    return getMax();
}

//----------------------------------------------------------
float ofBox::getHorzAnchor(ofAlignHorz anchor) const {
    switch (anchor) {
        case OF_ALIGN_HORZ_IGNORE:
            ofLogError("ofBox") << "getHorzAnchor(): unable to get anchor for OF_ALIGN_HORZ_IGNORE, returning 0";
            return 0.0f;
        case OF_ALIGN_HORZ_LEFT:
            return getLeft();
        case OF_ALIGN_HORZ_RIGHT:
            return getRight();
        case OF_ALIGN_HORZ_CENTER:
            return getCenter().x;
        default:
            ofLogError("ofBox") << "getHorzAnchor(): unknown ofAlignHorz = " << anchor << ", returning 0.0";
            return 0.0f;
    }
}

//----------------------------------------------------------
float ofBox::getVertAnchor(ofAlignVert anchor) const {
    switch (anchor) {
        case OF_ALIGN_VERT_IGNORE:
            ofLogError("ofBox") << "getVertAnchor(): unable to get anchor for OF_ALIGN_VERT_IGNORE, returning 0.0";
            return 0.0f;
        case OF_ALIGN_VERT_TOP:
            return getTop();
        case OF_ALIGN_VERT_BOTTOM:
            return getBottom();
        case OF_ALIGN_VERT_CENTER:
            return getCenter().y;
        default:
            ofLogError("ofBox") << "getVertAnchor(): unknown ofAlignVert = " << anchor << ", returning 0.0";
            return 0.0f;
    }
}
*/

//----------------------------------------------------------
bool ofBox::operator != (const ofBox& box) const {
	return	(x != box.x) || (y != box.y) || (z != box.z) ||
			(width != box.width) || (height != box.height) || (depth != box.depth);
}

//----------------------------------------------------------
ofPoint ofBox::getPosition() const {
    return position;
}
//----------------------------------------------------------
ofPoint& ofBox::getPositionRef() {
    return position;
}
//----------------------------------------------------------
ofPoint ofBox::getSize() const {
    return size;
}
//----------------------------------------------------------
ofPoint& ofBox::getSizeRef() {
    return size;
}

//----------------------------------------------------------
ofPoint ofBox::getCenter() const {
	return ofPoint(x + width * 0.5f, y + height * 0.5f, z + depth * 0.5f);
}

//----------------------------------------------------------
float ofBox::getX() const {
    return x;
}
//----------------------------------------------------------
float ofBox::getY() const {
    return y;
}
//----------------------------------------------------------
float ofBox::getZ() const {
    return z;
}
//----------------------------------------------------------
float ofBox::getWidth() const {
    return width;
}
//----------------------------------------------------------
float ofBox::getHeight() const {
    return height;
}
//----------------------------------------------------------
float ofBox::getDepth() const {
    return depth;
}

//----------------------------------------------------------
ofBox& ofBox::operator = (const ofBox& box) {
    set(box);
	return *this;
}

//----------------------------------------------------------
ofBox ofBox::operator + (const ofPoint & point){
	ofBox box=*this;
	box.x += point.x;
	box.y += point.y;
	box.z += point.z;
	return box;
}

//----------------------------------------------------------
bool ofBox::operator == (const ofBox& box) const {
	return	(x == box.x) && (y == box.y) && (z == box.z) &&
			(width == box.width) && (height == box.height) && (depth == box.depth);
}

//----------------------------------------------------------
ostream& operator<<(ostream& os, const ofBox& box){
	os << box.position << ", " << box.width << ", " << box.height << ", " << box.depth;
	return os;
}

//----------------------------------------------------------
istream& operator>>(istream& is, ofBox& box){
	is >> box.position;
	is.ignore(2);
	is >> box.width;
	is.ignore(2);
	is >> box.height;
	is.ignore(2);
	is >> box.depth;
	return is;
}
