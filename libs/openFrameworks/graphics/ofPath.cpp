/*
 * ofPath.cpp
 *
 *  Created on: 22/01/2011
 *      Author: arturo
 */
#include "ofPath.h"
#include "ofGraphics.h"

class ofPathToShapeConverter;
#include <deque>

ofPath::ofPath(){
	strokeWidth = 1;
	bFill = false;
	windingMode = OF_POLY_WINDING_ODD;
	bClosed = false;
	renderer = NULL;
	prevCurveRes = 16;
}

void ofPath::clear(){
	commands.clear();
	subPaths.clear();
	hasChanged = true;
}

ofPath & ofPath::newSubPath(){
	subPaths.push_back(ofPath());
	lastPath().windingMode = windingMode;
	lastPath().bFill = bFill;
	lastPath().strokeColor = strokeColor;
	lastPath().strokeWidth = strokeWidth;
	lastPath().fillColor = fillColor;
	return subPaths.back();
}

void ofPath::lineTo(const ofPoint & p){
	lastPath().addCommand(Command(Command::line3DTo,p));
}

void ofPath::lineTo(float x, float y, float z){
	lineTo(ofPoint(x,y,z));
}

void ofPath::lineTo(float x, float y){
	lastPath().addCommand(Command(Command::line2DTo,ofPoint(x,y)));
}

void ofPath::moveTo(const ofPoint & p){
	newSubPath().addCommand(Command(Command::line2DTo,p));
}

void ofPath::moveTo(float x, float y, float z){
	moveTo(ofPoint(x,y,z));
}

void ofPath::curveTo(const ofPoint & p){
	lastPath().addCommand(Command(Command::curve3DTo,p));
}

void ofPath::curveTo(float x, float y, float z){
	curveTo(ofPoint(x,y,z));
}

void ofPath::curveTo(float x, float y){
	lastPath().addCommand(Command(Command::curve2DTo,ofPoint(x,y)));
}

void ofPath::bezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	lastPath().addCommand(Command(Command::bezier3DTo,p,cp1,cp2));
}

void ofPath::bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	lastPath().addCommand(Command(Command::bezier2DTo,ofPoint(x,y),ofPoint(cx1,cy1),ofPoint(cx2,cy2)));
}

void ofPath::bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	bezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

void ofPath::quadBezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	lastPath().addCommand(Command(Command::quadBezier3DTo,p,cp1,cp2));
}

void ofPath::quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	lastPath().addCommand(Command(Command::quadBezier2DTo,ofPoint(x,y),ofPoint(cx1,cy1),ofPoint(cx2,cy2)));
}

void ofPath::quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	quadBezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

void ofPath::arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	lastPath().addCommand(Command(Command::arc3D,centre,radiusX,radiusY,angleBegin,angleEnd));
}

void ofPath::arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd){
	lastPath().addCommand(Command(Command::arc2D,ofPoint(x,y),radiusX,radiusY,angleBegin,angleEnd));
}

void ofPath::arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(ofPoint(x,y,z),radiusX,radiusY,angleBegin,angleEnd);
}

void ofPath::close(){
	lastPath().bClosed=true;
	lastPath().hasChanged = true;
	newSubPath();
}

void ofPath::setPolyWindingMode(ofPolyWindingMode mode){
	windingMode = mode;
	hasChanged = true;
}

void ofPath::setFilled(bool hasFill){
	bFill = hasFill;
	for(int i=0; i<subPaths.size(); i++){
		subPaths[i].setFilled(hasFill);
	}
}

void ofPath::setFillColor(const ofColor & color){
	fillColor = color;
	for(int i=0; i<subPaths.size(); i++){
		subPaths[i].setFillColor(color);
	}
}

void ofPath::setStrokeColor(const ofColor & color){
	strokeColor = color;
	for(int i=0; i<subPaths.size(); i++){
		subPaths[i].setStrokeColor(color);
	}
}

void ofPath::setStrokeWidth(float width){
	strokeWidth = width;
	for(int i=0; i<subPaths.size(); i++){
		subPaths[i].setStrokeWidth(width);
	}
}

ofPath & ofPath::lastPath(){
	if(subPaths.empty()){
		return *this;
	}else{
		return subPaths.back();
	}
}
const vector<ofPath::Command> & ofPath::getCommands() const{
	return commands;
}
vector<ofPath::Command> & ofPath::getCommands(){
	return commands;
}
vector<ofPath> & ofPath::getSubPaths(){
	return subPaths;
}

const vector<ofPath> & ofPath::getSubPaths() const{
	return subPaths;
}


ofPolyWindingMode ofPath::getWindingMode() const{
	return windingMode;
}

bool ofPath::isFilled() const{
	return bFill;
}

ofColor ofPath::getFillColor() const{
	return fillColor;
}

ofColor ofPath::getStrokeColor() const{
	return strokeColor;
}

float ofPath::getStrokeWidth() const{
	return strokeWidth;
}

bool ofPath::isClosed() const{
	return bClosed;
}

void ofPath::addCommand(const ofPath::Command & command){
	commands.push_back(command);
	hasChanged=true;
}

ofShape & ofPath::getShape(int curveResolution){
	if(curveResolution<=0) curveResolution=prevCurveRes;
	if(hasChanged || curveResolution!=prevCurveRes){
		prevCurveRes = curveResolution;
		cachedShape.setFrom(*this,curveResolution);
		hasChanged = false;
	}
	cachedShape.setStrokeColor(strokeColor);
	cachedShape.setStrokeWidth(strokeWidth);
	cachedShape.setFillColor(fillColor);
	cachedShape.setPolyWindingMode(windingMode);
	cachedShape.setFilled(bFill);
	return cachedShape;
}

void ofPath::updateShape(){
	getShape();
}

void ofPath::draw(float x, float y){
	ofPushMatrix();
	ofTranslate(x,y);
	if(!renderer){
		ofGetDefaultRenderer()->draw(*this);
	}else{
		renderer->draw(*this);
	}
	ofPopMatrix();
}


void ofPath::markedChanged(){
	hasChanged=true;
}
