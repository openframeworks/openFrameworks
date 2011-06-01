#include "ofRectangle.h"

ofRectangle::ofRectangle (){ 
	x = y = width = height = 0.0f; 
}

ofRectangle::~ ofRectangle (){}

ofRectangle::ofRectangle (float _x, float _y, float _w, float _h){
	x = _x;
	y = _y;
	width = _w;
	height = _h;
}

ofRectangle::ofRectangle (ofRectangle const & r){
	x = r.x;
	y = r.y;
	width = r.width;
	height = r.height;
}

void ofRectangle::set (float px, float py, float w, float h){
	x		= px;
	y		= py;
	width	= w;
	height	= h;
}

void ofRectangle::set (ofPoint pos, float w, float h){
	x		= pos.x;
	y		= pos.y;
	width	= w;
	height	= h;
}

void ofRectangle::set (ofRectangle const & rect){
	x		= rect.x;
	y		= rect.y;
	width	= rect.width;
	height	= rect.height;
}

void ofRectangle::setFromCenter (float px, float py, float w, float h){
	x		= px - w*0.5f;
	y		= py - h*0.5f;
	width	= w;
	height	= h;
}

void ofRectangle::setFromCenter (ofPoint pos, float w, float h){
	x		= pos.x - w*0.5f;
	y		= pos.y - h*0.5f;
	width	= w;
	height	= h;
}

ofPoint ofRectangle::getCenter (){
	return ofPoint(x + width * 0.5f, y + height * 0.5f, 0);
}

bool ofRectangle::inside (ofPoint p){
	return inside(p.x, p.y);
}

bool ofRectangle::inside (float px, float py){
	if( px > x && py > y && px < x + width && py < y + height ){
		return true;
	}
	return false;
}

ofRectangle & ofRectangle::operator = (ofRectangle const & rect){
	x = rect.x;
	y = rect.y;
	width = rect.width;
	height = rect.height;
	return *this;
}

ofRectangle & ofRectangle::operator + (const ofPoint & point){
	x += point.x;
	y += point.y;
	return *this;
}

bool ofRectangle::operator == (ofRectangle const & r){
	return (x == r.x) && (y == r.y) && (width == r.width) && (height == r.height);
}

bool ofRectangle::operator != (ofRectangle const & r){
	return (x != r.x) || (y != r.y) || (width != r.width) || (height != r.height);
}

