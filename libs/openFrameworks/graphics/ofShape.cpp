#include "ofShape.h"
#include "ofGraphics.h"

class ofCommandToShapeConverter;
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
	subShapes.clear();
	hasChanged = true;
}

ofShape & ofShape::newSubShape(){
	subShapes.push_back(ofShape());
	lastShape().windingMode = windingMode;
	lastShape().bFill = bFill;
	lastShape().strokeColor = strokeColor;
	lastShape().strokeWidth = strokeWidth;
	lastShape().fillColor = fillColor;
	return subShapes.back();
}

void ofShape::lineTo(const ofPoint & p){
	lastShape().addCommand(Command(Command::lineTo,p));
}

void ofShape::lineTo(float x, float y, float z){
	lineTo(ofPoint(x,y,z));
}

void ofShape::lineTo(float x, float y){
	lastShape().addCommand(Command(Command::lineTo,ofPoint(x,y)));
}

void ofShape::moveTo(const ofPoint & p){
	newSubShape().addCommand(Command(Command::lineTo,p));
}

void ofShape::moveTo(float x, float y, float z){
	moveTo(ofPoint(x,y,z));
}

void ofShape::curveTo(const ofPoint & p){
	lastShape().addCommand(Command(Command::curveTo,p));
}

void ofShape::curveTo(float x, float y, float z){
	curveTo(ofPoint(x,y,z));
}

void ofShape::curveTo(float x, float y){
	lastShape().addCommand(Command(Command::curveTo,ofPoint(x,y)));
}

void ofShape::bezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	lastShape().addCommand(Command(Command::bezierTo,p,cp1,cp2));
}

void ofShape::bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	lastShape().addCommand(Command(Command::bezierTo,ofPoint(x,y),ofPoint(cx1,cy1),ofPoint(cx2,cy2)));
}

void ofShape::bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	bezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

void ofShape::quadBezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	lastShape().addCommand(Command(Command::quadBezierTo,p,cp1,cp2));
}

void ofShape::quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	lastShape().addCommand(Command(Command::quadBezierTo,ofPoint(x,y),ofPoint(cx1,cy1),ofPoint(cx2,cy2)));
}

void ofShape::quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	quadBezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

void ofShape::arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	lastShape().addCommand(Command(Command::arc,centre,radiusX,radiusY,angleBegin,angleEnd));
}

void ofShape::arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd){
	lastShape().addCommand(Command(Command::arc,ofPoint(x,y),radiusX,radiusY,angleBegin,angleEnd));
}

void ofShape::arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(ofPoint(x,y,z),radiusX,radiusY,angleBegin,angleEnd);
}

void ofShape::close(){
	lastShape().bClosed=true;
	lastShape().hasChanged = true;
	newSubShape();
}

void ofShape::setPolyWindingMode(ofPolyWindingMode mode){
	windingMode = mode;
	hasChanged = true;
}

void ofShape::setFilled(bool hasFill){
	bFill = hasFill;
	for(int i=0; i<(int)subShapes.size(); i++){
		subShapes[i].setFilled(hasFill);
	}
}

void ofShape::setFillColor(const ofColor & color){
	fillColor = color;
	for(int i=0; i<(int)subShapes.size(); i++){
		subShapes[i].setFillColor(color);
	}
}

void ofShape::setStrokeColor(const ofColor & color){
	strokeColor = color;
	for(int i=0; i<(int)subShapes.size(); i++){
		subShapes[i].setStrokeColor(color);
	}
}

void ofShape::setStrokeWidth(float width){
	strokeWidth = width;
	for(int i=0; i<(int)subShapes.size(); i++){
		subShapes[i].setStrokeWidth(width);
	}
}

ofShape & ofShape::lastShape(){
	if(subShapes.empty()){
		return *this;
	}else{
		return subShapes.back();
	}
}
const vector<ofShape::Command> & ofShape::getCommands() const{
	return commands;
}
vector<ofShape::Command> & ofShape::getCommands(){
	return commands;
}
vector<ofShape> & ofShape::getSubShapes(){
	return subShapes;
}

const vector<ofShape> & ofShape::getSubShapes() const{
	return subShapes;
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

ofShapeTessellation & ofShape::getTessellation(int curveResolution){
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
	getTessellation();
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
