#include "ofShape.h"
#include "ofGraphics.h"

class ofCommandToShapeConverter;
#include <deque>

ofPath::ofPath(){
	bClosed = false;
	bHasChanged = true;
}
const vector<ofPath::Command> & ofPath::getCommands() const{
	return commands;
}

vector<ofPath::Command> & ofPath::getCommands(){
	return commands;
}

void ofPath::addCommand(const ofPath::Command & command){
	commands.push_back(command);
	bHasChanged=true;
}

void ofPath::close(){
	bClosed = true;
}

bool ofPath::isClosed(){
	return bClosed;
}

ofShape::ofShape(){
	strokeWidth = 1;
	bFill = false;
	windingMode = OF_POLY_WINDING_ODD;
	renderer = NULL;
	prevCurveRes = 16;
	mode = PATHS;
	hasChanged = false;
}

void ofShape::clear(){
	paths.clear();
	hasChanged = true;
}

void ofShape::newPath(){
	if(mode==PATHS){
		paths.push_back(ofPath());
	}else{
		polylines.push_back(ofPolyline());
	}
	bNeedsTessellation = true;
}

void ofShape::lineTo(const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofPath::Command(ofPath::Command::lineTo,p));
	}else{
		lastPolyline().lineTo(p);
	}
}

void ofShape::lineTo(float x, float y, float z){
	lineTo(ofPoint(x,y,z));
}

void ofShape::lineTo(float x, float y){
	lineTo(ofPoint(x,y,0));
}

void ofShape::moveTo(const ofPoint & p){
	newPath();
	if(mode==PATHS){
		lastPath().addCommand(ofPath::Command(ofPath::Command::lineTo,p));
	}else{
		lastPolyline().addVertex(p);
	}
}

void ofShape::moveTo(float x, float y, float z){
	moveTo(ofPoint(x,y,z));
}

void ofShape::curveTo(const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofPath::Command(ofPath::Command::curveTo,p));
	}else{
		lastPolyline().curveTo(p);
	}
}

void ofShape::curveTo(float x, float y, float z){
	curveTo(ofPoint(x,y,z));
}

void ofShape::curveTo(float x, float y){
	curveTo(ofPoint(x,y,0));
}

void ofShape::bezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofPath::Command(ofPath::Command::bezierTo,p,cp1,cp2));
	}else{
		lastPolyline().bezierTo(cp1,cp2,p);
	}
}

void ofShape::bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	bezierTo(ofPoint(cx1,cy1,0),ofPoint(cx2,cy2,0),ofPoint(x,y,0));
}

void ofShape::bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	bezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

void ofShape::quadBezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofPath::Command(ofPath::Command::quadBezierTo,p,cp1,cp2));
	}else{
		lastPolyline().quadBezierTo(cp1,cp2,p);
	}
}

void ofShape::quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	quadBezierTo(ofPoint(cx1,cy1,0),ofPoint(cx2,cy2,0),ofPoint(x,y,0));
}

void ofShape::quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	quadBezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

void ofShape::arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	if(mode==PATHS){
		lastPath().addCommand(ofPath::Command(ofPath::Command::arc,centre,radiusX,radiusY,angleBegin,angleEnd));
	}else{
		lastPolyline().arc(centre,radiusX,radiusY,angleBegin,angleEnd);
	}
}

void ofShape::arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(ofPoint(x,y,0),radiusX,radiusY,angleBegin,angleEnd);
}

void ofShape::arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(ofPoint(x,y,z),radiusX,radiusY,angleBegin,angleEnd);
}

void ofShape::close(){
	if(mode==PATHS){
		lastPath().close();
	}else{
		lastPolyline().setClosed(true);
	}
}

void ofShape::setPolyWindingMode(ofPolyWindingMode mode){
	windingMode = mode;
	hasChanged = true;
}

void ofShape::setFilled(bool hasFill){
	bFill = hasFill;
}

void ofShape::setFillColor(const ofColor & color){
	fillColor = color;
}

void ofShape::setStrokeColor(const ofColor & color){
	strokeColor = color;
}

void ofShape::setStrokeWidth(float width){
	strokeWidth = width;
}

ofPath & ofShape::lastPath(){
	if(paths.empty()){
		paths.push_back(ofPath());
	}
	return paths.back();
}

ofPolyline & ofShape::lastPolyline(){
	if(polylines.empty()){
		polylines.push_back(ofPolyline());
	}
	return polylines.back();
}

vector<ofPath> & ofShape::getPaths(){
	if(mode==POLYLINES){
		ofLog(OF_LOG_WARNING,"trying to get paths from shape with polylines only");
	}
	return paths;
}

const vector<ofPath> & ofShape::getPaths() const{
	if(mode==POLYLINES){
		ofLog(OF_LOG_WARNING,"trying to get paths from shape with polylines only");
	}
	return paths;
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


ofShapeTessellation & ofShape::getTessellation(int curveResolution){
	/*if(curveResolution<=0) curveResolution=prevCurveRes;
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
	return cachedShape;*/
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
