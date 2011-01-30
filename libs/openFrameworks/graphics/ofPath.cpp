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

ofShape::ofShape(){
	strokeWidth = 1;
	bFill = false;
	windingMode = OF_POLY_WINDING_ODD;
	bClosed = false;
	renderer = NULL;
	prevCurveRes = 16;
}

void ofShape::clear(){
	commands.clear();
	subPaths.clear();
	hasChanged = true;
}

ofShape & ofShape::newSubPath(){
	subPaths.push_back(ofShape());
	lastPath().windingMode = windingMode;
	lastPath().bFill = bFill;
	lastPath().strokeColor = strokeColor;
	lastPath().strokeWidth = strokeWidth;
	lastPath().fillColor = fillColor;
	return subPaths.back();
}

void ofShape::lineTo(const ofPoint & p){
	lastPath().addCommand(Command(Command::line3DTo,p));
}

void ofShape::lineTo(float x, float y, float z){
	lineTo(ofPoint(x,y,z));
}

void ofShape::lineTo(float x, float y){
	lastPath().addCommand(Command(Command::line2DTo,ofPoint(x,y)));
}

void ofShape::moveTo(const ofPoint & p){
	newSubPath().addCommand(Command(Command::line2DTo,p));
}

void ofShape::moveTo(float x, float y, float z){
	moveTo(ofPoint(x,y,z));
}

void ofShape::curveTo(const ofPoint & p){
	lastPath().addCommand(Command(Command::curve3DTo,p));
}

void ofShape::curveTo(float x, float y, float z){
	curveTo(ofPoint(x,y,z));
}

void ofShape::curveTo(float x, float y){
	lastPath().addCommand(Command(Command::curve2DTo,ofPoint(x,y)));
}

void ofShape::bezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	lastPath().addCommand(Command(Command::bezier3DTo,p,cp1,cp2));
}

void ofShape::bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	lastPath().addCommand(Command(Command::bezier2DTo,ofPoint(x,y),ofPoint(cx1,cy1),ofPoint(cx2,cy2)));
}

void ofShape::bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	bezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

void ofShape::quadBezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	lastPath().addCommand(Command(Command::quadBezier3DTo,p,cp1,cp2));
}

void ofShape::quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	lastPath().addCommand(Command(Command::quadBezier2DTo,ofPoint(x,y),ofPoint(cx1,cy1),ofPoint(cx2,cy2)));
}

void ofShape::quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	quadBezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

void ofShape::arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	lastPath().addCommand(Command(Command::arc3D,centre,radiusX,radiusY,angleBegin,angleEnd));
}

void ofShape::arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd){
	lastPath().addCommand(Command(Command::arc2D,ofPoint(x,y),radiusX,radiusY,angleBegin,angleEnd));
}

void ofShape::arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(ofPoint(x,y,z),radiusX,radiusY,angleBegin,angleEnd);
}

void ofShape::close(){
	lastPath().bClosed=true;
	lastPath().hasChanged = true;
	newSubPath();
}

void ofShape::setPolyWindingMode(ofPolyWindingMode mode){
	windingMode = mode;
	hasChanged = true;
}

void ofShape::setFilled(bool hasFill){
	bFill = hasFill;
	for(int i=0; i<subPaths.size(); i++){
		subPaths[i].setFilled(hasFill);
	}
}

void ofShape::setFillColor(const ofColor & color){
	fillColor = color;
	for(int i=0; i<subPaths.size(); i++){
		subPaths[i].setFillColor(color);
	}
}

void ofShape::setStrokeColor(const ofColor & color){
	strokeColor = color;
	for(int i=0; i<subPaths.size(); i++){
		subPaths[i].setStrokeColor(color);
	}
}

void ofShape::setStrokeWidth(float width){
	strokeWidth = width;
	for(int i=0; i<subPaths.size(); i++){
		subPaths[i].setStrokeWidth(width);
	}
}

ofShape & ofShape::lastPath(){
	if(subPaths.empty()){
		return *this;
	}else{
		return subPaths.back();
	}
}
const vector<ofShape::Command> & ofShape::getCommands() const{
	return commands;
}
vector<ofShape::Command> & ofShape::getCommands(){
	return commands;
}
vector<ofShape> & ofShape::getSubPaths(){
	return subPaths;
}

const vector<ofShape> & ofShape::getSubPaths() const{
	return subPaths;
}


ofPolyWindingMode ofShape::getWindingMode() const{
	return windingMode;
}

bool ofShape::isFilled() const{
	return bFill;
}

ofColor ofShape::getFillColor() const{
	return fillColor;
}

ofColor ofShape::getStrokeColor() const{
	return strokeColor;
}

float ofShape::getStrokeWidth() const{
	return strokeWidth;
}

bool ofShape::isClosed() const{
	return bClosed;
}

void ofShape::addCommand(const ofShape::Command & command){
	commands.push_back(command);
	hasChanged=true;
}

ofShapeTessellation & ofShape::getShape(int curveResolution){
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

void ofShape::updateShape(){
	getShape();
}

void ofShape::draw(float x, float y){
	ofPushMatrix();
	ofTranslate(x,y);
	if(!renderer){
		ofGetDefaultRenderer()->draw(*this);
	}else{
		renderer->draw(*this);
	}
	ofPopMatrix();
}


void ofShape::markedChanged(){
	hasChanged=true;
}
