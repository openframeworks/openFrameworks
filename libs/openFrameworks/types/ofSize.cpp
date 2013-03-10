//
//  ofSize.cpp
//  openFrameworksLib
//
//  Created by ISHII 2bit on 2013/03/11.
//
//

#include "ofSize.h"

ofSize::ofSize() {
    set(0, 0);
}

ofSize::~ofSize() {}

ofSize::ofSize(float w, float h) {
    set(w, h);
}

void ofSize::set(float w ,float h) {
    width  = w;
    height = h;
}

void ofSize::set(const ofSize& size) {
    set(size.width, size.height);
}

void ofSize::setWidth(float w) {
    width = w;
}

void ofSize::setHeight(float h) {
    height = h;
}

void ofSize::scale(float s) {
    scaleWidth(s);
    scaleHeight(s);
}

void ofSize::scale(float sX, float sY) {
    scaleWidth(sX);
    scaleHeight(sX);
}

void ofSize::scaleWidth(float sX) {
    width *= sX;
}

void ofSize::scaleHeight(float sY) {
    height *= sY;
}

ofSize ofSize::getScaled(float s) const {
    return getScaled(s, s);
}

ofSize ofSize::getScaled(float sX, float sY) const {
    ofSize newSize = *this;
    newSize.scale(sX, sY);
    return newSize;
}

ofSize ofSize::getScaleWidth(float sX) const {
    return getScaled(sX, 1);
}

ofSize ofSize::getScaleHeight(float sY) const {
    return getScaled(1, sY);
}

float ofSize::getArea() const {
    return fabs(width * height);
}

float ofSize::getPerimeter() const {
    return 2.0f * fabs(width) + 2.0f * fabs(height);
}

float ofSize::getAspectRatio() const {
    return fabs(width) / fabs(height);
}

bool ofSize::isEmpty() const {
    return width == 0.0f && height == 0.0f;
}

float ofSize::getWidth() const {
    return width;
}

float ofSize::getHeight() const {
    return height;
}

bool ofSize::operator != (const ofSize& size) const {
	return (width != size.width) || (height != size.height);
}

//----------------------------------------------------------
bool ofSize::operator == (const ofSize& size) const {
	return !(*this != size);
}

//----------------------------------------------------------
ofSize& ofSize::operator = (const ofSize& size) {
    set(size);
	return *this;
}

//----------------------------------------------------------
ofSize& ofSize::operator * (float s){
    width *= s;
    height *= s;
	return *this;
}