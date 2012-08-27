#include <cfloat>
#include "ofRectangle.h"
 
//----------------------------------------------------------
ofRectangle::ofRectangle(){
    set(0,0,0,0);
}

//----------------------------------------------------------
ofRectangle::~ ofRectangle(){}

//----------------------------------------------------------
ofRectangle::ofRectangle(float px, float py, float w, float h){
	set(px,py,w,h);
}

//----------------------------------------------------------
ofRectangle::ofRectangle(const ofPoint& p, float w, float h){
    set(p,w,h);
}

//----------------------------------------------------------
ofRectangle::ofRectangle(const ofRectangle& rect){
    set(rect);
}

//----------------------------------------------------------
ofRectangle::ofRectangle(const ofPoint& p0, const ofPoint& p1) {
    set(p0,p1);
}

//----------------------------------------------------------
void ofRectangle::set(float px, float py, float w, float h){
	x		= px;
	y		= py;
	width	= w;
	height	= h;
}

//----------------------------------------------------------
void ofRectangle::set(const ofPoint& p, float w, float h){
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
void ofRectangle::setFromCenter(float px, float py, float w, float h){
    set(px - w*0.5f, py - h*0.5f, w, h);
}

//----------------------------------------------------------
void ofRectangle::setFromCenter(const ofPoint& p, float w, float h){
    setFromCenter(p.x, p.y, w, h);
}

//----------------------------------------------------------
ofPoint ofRectangle::getCenter() const {
	return ofPoint(x + width * 0.5f, y + height * 0.5f, 0);
}

//----------------------------------------------------------
void ofRectangle::translate(float px, float py) {
    x += px;
    y += py;
}

//----------------------------------------------------------
void ofRectangle::translate(const ofPoint& p) {
    translate(p.x,p.y);
}

//----------------------------------------------------------
void ofRectangle::scale(float s) {
    scale(s,s);
}

//----------------------------------------------------------
void ofRectangle::scale(float sX, float sY) {
    width  *= sX;
    height *= sY;
}

//----------------------------------------------------------
void ofRectangle::scale(const ofPoint& s) {
    scale(s.x,s.y);
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
    if(s.x == 1.0f && s.y == 1.0f) return;
  
    canonicalize(); // will canonicalize

    float newWidth  = width  * s.x;
    float newHeight = height * s.y;

    ofPoint center = getCenter();
    
    x = center.x - newWidth / 2.0f; 
    y = center.y - newHeight / 2.0f; 
    
    width  = newWidth;
    height = newHeight;

}

//----------------------------------------------------------
ofRectangle ofRectangle::scaleToMe(const ofRectangle& toBeScaled, ofRectScaleMode rectScaleMode) const {

    ofRectangle result;

    // the easy case shortcuts
    if(rectScaleMode == OF_RECTSCALEMODE_STRETCH) {
        result.set(*this);
        return result;
    } else if(rectScaleMode == OF_RECTSCALEMODE_CENTER) {
        result.setFromCenter(getCenter(), // take a new center
                             toBeScaled.getWidth(), // keep original width
                             toBeScaled.getHeight()); // keep original height
        return result;
    }

    float _x  = getX();
    float _y  = getY();
    float w  = getWidth();
    float h  = getHeight();
    float fw = toBeScaled.getWidth();
    float fh = toBeScaled.getHeight();

    // make sure we are dealing with non-zero rects, else divide by zero
    if(fabs(fw) < FLT_EPSILON ||
       fabs(fh) < FLT_EPSILON) {
        ofLogWarning("ofRectangle") << "Source rectangle had 0 width or 0 height. Avoiding divide by zero.";
        return result;
    }
    
    float resultScale;
    
    // modifiy the result scale if needed
    if(rectScaleMode == OF_RECTSCALEMODE_FILL) {
        resultScale = MAX(fabs(w) / fabs(fw),
                          fabs(h) / fabs(fh));
    } else if(rectScaleMode == OF_RECTSCALEMODE_FIT) {
        resultScale = MIN(fabs(w) / fabs(fw),
                          fabs(h) / fabs(fh));
    } else {
        ofLogWarning("ofRectangle") << "Encountered unknown ofRectScaleMode.  Using OF_RECTSCALEMODE_FIT.";
        resultScale = MIN(fabs(w) / fabs(fw),
                          fabs(h) / fabs(fh));
    }

    result.set(_x + ( w - fw * resultScale ) * 0.5f,
               _y + ( h - fh * resultScale ) * 0.5f,
                        ( fw * resultScale ),
                        ( fh * resultScale ));
    
    return result;
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
bool ofRectangle::intersects(const ofRectangle& rect) const {
    return (getMinX() < rect.getMaxX() && getMaxX() > rect.getMinX() &&
            getMinY() < rect.getMaxY() && getMaxY() > rect.getMinY());
}

//----------------------------------------------------------
void ofRectangle::growToInclude(float px, float py){
    growToInclude(ofPoint(px,py));
}

//----------------------------------------------------------
void ofRectangle::growToInclude(const ofPoint& p){
    float x0 = MIN(getMinX(),p.x);
    float x1 = MAX(getMaxX(),p.x);
    float y0 = MIN(getMinY(),p.y);
    float y1 = MAX(getMaxY(),p.y);
    float w = x1 - x0;
    float h = y1 - y0;
    set(x0,y0,w,h);
}

//----------------------------------------------------------
void ofRectangle::growToInclude(const ofRectangle& rect){
    float x0 = MIN(getMinX(),rect.getMinX());
    float x1 = MAX(getMaxX(),rect.getMaxX());
    float y0 = MIN(getMinY(),rect.getMinY());
    float y1 = MAX(getMaxY(),rect.getMaxY());
    float w = x1 - x0;
    float h = y1 - y0;
    set(x0,y0,w,h);
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
void ofRectangle::canonicalize() {
    if(width < 0.0f) {
        x += width;
        width = -1.0 * width;
    } 
    
    if(height < 0.0f) {
        y += height;
        height = -1.0 * height;
    }
}

//----------------------------------------------------------
ofRectangle ofRectangle::getCanonicalized() const {
    ofRectangle canRect(*this); // copy it
    canRect.canonicalize();
    return canRect;
}

//----------------------------------------------------------
bool ofRectangle::isCanonicalized() const {
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
ofRectangle& ofRectangle::operator = (const ofRectangle& rect) {
    set(rect);
	return *this;
}

//----------------------------------------------------------
ofRectangle & ofRectangle::operator + (const ofPoint & point){
	x += point.x;
	y += point.y;
	return *this;
}

//----------------------------------------------------------
bool ofRectangle::operator == (const ofRectangle& rect) const {
	return (x == rect.x) && (y == rect.y) && (width == rect.width) && (height == rect.height);
}

//----------------------------------------------------------
bool ofRectangle::operator != (const ofRectangle& rect) const {
	return (x != rect.x) || (y != rect.y) || (width != rect.width) || (height != rect.height);
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