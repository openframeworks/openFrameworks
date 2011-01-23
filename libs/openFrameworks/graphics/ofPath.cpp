/*
 * ofPath.cpp
 *
 *  Created on: 22/01/2011
 *      Author: arturo
 */
#include "ofPath.h"

class ofPathToShapeConverter;

ofPath::ofPath(){
	strokeWidth = 1;
	bFill = true;
	windingMode = OF_POLY_WINDING_ODD;
	bClosed = false;
}

void ofPath::clear(){
	commands.clear();
	subPaths.clear();
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
	lastPath().commands.push_back(Command(Command::lineTo,p));
}

void ofPath::lineTo(float x, float y, float z){
	lineTo(ofPoint(x,y,z));
}

void ofPath::moveTo(const ofPoint & p){
	newSubPath().commands.push_back(Command(Command::lineTo,p));
}

void ofPath::moveTo(float x, float y, float z){
	moveTo(ofPoint(x,y,z));
}

void ofPath::curveTo(const ofPoint & p){
	lastPath().commands.push_back(Command(Command::curveTo,p));
}

void ofPath::curveTo(float x, float y, float z){
	curveTo(ofPoint(x,y,z));
}

void ofPath::bezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	lastPath().commands.push_back(Command(Command::bezier3DTo,p,cp1,cp2));
}

void ofPath::bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	lastPath().commands.push_back(Command(Command::bezier2DTo,ofPoint(x,y),ofPoint(cx1,cy1),ofPoint(cx2,cy2)));
}

void ofPath::bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	bezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

void ofPath::arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	lastPath().commands.push_back(Command(Command::arc3D,centre,radiusX,radiusY,angleBegin,angleEnd));
}

void ofPath::arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd){
	lastPath().commands.push_back(Command(Command::arc2D,ofPoint(x,y),radiusX,radiusY,angleBegin,angleEnd));
}

void ofPath::arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(ofPoint(x,y,z),radiusX,radiusY,angleBegin,angleEnd);
}

void ofPath::close(){
	lastPath().bClosed=true;
	newSubPath();
}

void ofPath::setPolyWindingMode(ofPolyWindingMode mode){
	lastPath().windingMode = mode;
}

void ofPath::setFilled(bool hasFill){
	lastPath().bFill = hasFill;
}

void ofPath::setFillColor(const ofColor & color){
	lastPath().fillColor = color;
}

void ofPath::setStrokeColor(const ofColor & color){
	lastPath().strokeColor = color;
}

void ofPath::setStrokeWidth(float width){
	lastPath().strokeWidth = width;
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

class ofPathToShapeConverter{
public:
	ofShape toShape(const ofPath & path, int curveResolution, bool tesselate=false){
		// TODO: goes trough all commands and subshapes commands
		// converting them to polylines and storing them in a shape and
		// subshapes
		ofShape shape;
		ofPolyline currentPolyline;
		const vector<ofPath::Command> & commands = path.getCommands();
		for(int i=0; i<(int)commands.size();i++){
			switch(commands[i].type){
			case ofPath::Command::lineTo:
				curveVertices.clear();
				currentPolyline.addVertex(commands[i].to);
				break;
			case ofPath::Command::curveTo:
				curveToVertices(currentPolyline,commands[i].to, curveResolution);
				break;
			case ofPath::Command::bezier2DTo:
				curveVertices.clear();
				bezier2DToVertices(currentPolyline,commands[i].controlPoints[0],commands[i].controlPoints[1],commands[i].to, curveResolution);
				break;
			case ofPath::Command::arc2D:
				curveVertices.clear();
				arcToVertices(currentPolyline,commands[i].to,commands[i].additionalParams[0],commands[i].additionalParams[1],commands[i].additionalParams[2],commands[i].additionalParams[3], curveResolution);
				break;
			}
		}
		currentPolyline.setClosed(path.isClosed());
		shape.setPolyline(currentPolyline);
		shape.setFillColor(path.getFillColor());
		shape.setLineColor(path.getStrokeColor());
		shape.setPolyWindingMode(path.getWindingMode());
		shape.setFilled(path.isFilled());
		//TODO: stroke width;

		const vector<ofPath> & subPaths = path.getSubPaths();
		for(int i=0; i<(int)subPaths.size(); i++){
			shape.addSubShape(toShape(subPaths[i],curveResolution,tesselate));
		}
		return shape;
	}

	void bezier2DToVertices( ofPolyline& polyline, const ofPoint & cp1, const ofPoint & cp2, const ofPoint & to, int curveResolution ){
		// if, and only if poly vertices has points, we can make a bezier
		// from the last point
		curveVertices.clear();

		// the resolultion with which we computer this bezier
		// is arbitrary, can we possibly make it dynamic?

		if (polyline.size() > 0){
			float x0 = polyline[polyline.size()-1].x;
			float y0 = polyline[polyline.size()-1].y;

			float   ax, bx, cx;
			float   ay, by, cy;
			float   t, t2, t3;
			float   x, y;

			// polynomial coefficients
			cx = 3.0f * (cp1.x - x0);
			bx = 3.0f * (cp2.x - cp1.x) - cx;
			ax = to.x - x0 - cx - bx;

			cy = 3.0f * (cp1.y - y0);
			by = 3.0f * (cp2.y - cp1.y) - cy;
			ay = to.y - y0 - cy - by;


			for (int i = 0; i < curveResolution; i++){
				t 	=  (float)i / (float)(curveResolution-1);
				t2 = t * t;
				t3 = t2 * t;
				x = (ax * t3) + (bx * t2) + (cx * t) + x0;
				y = (ay * t3) + (by * t2) + (cy * t) + y0;
				polyline.addVertex(x,y);
			}
		}
	}



	void curveToVertices( ofPolyline& polyline, const ofPoint & to, int curveResolution ){

		curveVertices.push_back(to);

		if (curveVertices.size() >= 4){

			int startPos = (int)curveVertices.size() - 4;

			float x0 = curveVertices[startPos + 0].x;
			float y0 = curveVertices[startPos + 0].y;
			float x1 = curveVertices[startPos + 1].x;
			float y1 = curveVertices[startPos + 1].y;
			float x2 = curveVertices[startPos + 2].x;
			float y2 = curveVertices[startPos + 2].y;
			float x3 = curveVertices[startPos + 3].x;
			float y3 = curveVertices[startPos + 3].y;

			float t,t2,t3;
			float x,y;

			for (int i = 0; i < curveResolution; i++){

				t 	=  (float)i / (float)(curveResolution-1);
				t2 	= t * t;
				t3 	= t2 * t;

				x = 0.5f * ( ( 2.0f * x1 ) +
				( -x0 + x2 ) * t +
				( 2.0f * x0 - 5.0f * x1 + 4 * x2 - x3 ) * t2 +
				( -x0 + 3.0f * x1 - 3.0f * x2 + x3 ) * t3 );

				y = 0.5f * ( ( 2.0f * y1 ) +
				( -y0 + y2 ) * t +
				( 2.0f * y0 - 5.0f * y1 + 4 * y2 - y3 ) * t2 +
				( -y0 + 3.0f * y1 - 3.0f * y2 + y3 ) * t3 );

				polyline.addVertex(ofPoint(x,y));
			}
		}
	}

	void arcToVertices( ofPolyline& polyline, const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int curveResolution){
		float size = (angleEnd - angleBegin)/360.0f;
		float begin = angleBegin/360.0f;
		float radius = radiusX;
		const int segs = fabs(size) * (200 * sqrt(radius));//(radius*2*M_PI) * 50;
		const int segments = (segs < curveResolution)? curveResolution : segs;

		float angle,sinus,cosinus;
		float segment_size = M_PI*2.0*size/(float)segments;
		angle=-(M_PI*2.0*begin);
		for( int i=0; i<segments; i++){
			sinus = sin(angle);
			cosinus = cos(angle);
			polyline.addVertex(radius*sinus,radius*cosinus);

			angle-=segment_size ;
		}
		angle=-(M_PI*2.0*begin);
		sinus = sin(angle);
		cosinus = cos(angle);
		polyline.addVertex(radius*sinus,radius*cosinus);
	}

private:
	vector<ofPoint> curveVertices;
};




ofShape ofPath::getShape(int curveResolution, bool tesselated){
	return ofPathToShapeConverter().toShape(*this,curveResolution,tesselated);
}

