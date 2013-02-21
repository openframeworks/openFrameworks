#include "testApp.h"

using namespace ofxCv;
using namespace cv;

#include "GeometryHelpers.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetLineWidth(3);
	ofSetFrameRate(120);
}

void testApp::update() {
	
}

void testApp::draw() {
	ofBackground(0);
	ofSetColor(255, 64);
	polyline.draw();
	
	ofSetColor(255, 32);
	float lineSum = 0, rectSum = 0, ellipseSum = 0;
	for(int i = 0; i < polyline.size(); i++) {
		lineSum += distanceToLine(polyline[i], linePoint - lineDirection * 1000, linePoint + lineDirection * 1000);
		rectSum += distanceToRect(polyline[i], rect);
		ellipseSum += distanceToEllipse(polyline[i], ellipse);
	}
	float perimeter = polyline.getPerimeter();
	lineSum /= perimeter;
	rectSum /= perimeter;
	ellipseSum /= perimeter;
	
	ofSetColor(255);
	ofDrawBitmapString("line: " + ofToString(lineSum), 10, 20);
	ofDrawBitmapString("rect: " + ofToString(rectSum), 10, 40);
	ofDrawBitmapString("ellipse: " + ofToString(ellipseSum), 10, 60);
	
	ofNoFill();
	
	float lineRatio = 6;
	bool lineLike = rect.size.width / rect.size.height > lineRatio || rect.size.height / rect.size.width > lineRatio;
	
	ofSetColor(255, 32);
	ofColor lineColor(255, 32), rectColor(255, 32), ellipseColor(255, 32);
	if(lineLike || (lineSum < rectSum && lineSum < ellipseSum)) {
		lineColor = magentaPrint;
		for(int i = 0; i < polyline.size(); i++) {
			ofLine(polyline[i], closestPointOnLine(linePoint + lineDirection * 1000, linePoint - lineDirection * 1000, polyline[i]));
		}
	} else if(rectSum < lineSum && rectSum < ellipseSum) {
		rectColor = yellowPrint;
		for(int i = 0; i < polyline.size(); i++) {
			ofLine(polyline[i], closestPointOnRect(rect, polyline[i]));
		}
	} else if(ellipseSum < lineSum && ellipseSum < rectSum) {
		ellipseColor = cyanPrint;
		for(int i = 0; i < polyline.size(); i++) {
			ofLine(polyline[i], closestPointOnEllipse(ellipse, polyline[i]));
		}
	}
	
	ofSetColor(lineColor);
	ofLine(linePoint, linePoint - lineDirection * 1000);
	ofLine(linePoint, linePoint + lineDirection * 1000);
	
	ofPushMatrix();
	ofTranslate(rect.center.x, rect.center.y);
	ofRotate(rect.angle);
	ofSetColor(rectColor);
	ofRect(-rect.size.width / 2, -rect.size.height / 2, rect.size.width, rect.size.height);
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(ellipse.center.x, ellipse.center.y);
	ofRotate(ellipse.angle);
	ofSetColor(ellipseColor);
	//ofEllipse(0, 0, ellipse.size.width, ellipse.size.height);
	if(polyline.size() > 1) {
		ofVec2f center(ellipse.center.x, ellipse.center.y);
		ofVec2f startPoint = polyline[0];
		startPoint -= center;
		startPoint.rotate(-ellipse.angle);
		ofVec2f stopPoint = polyline[polyline.size() - 1];
		stopPoint -= center;
		stopPoint.rotate(-ellipse.angle);
		float startAngle = atan2f(startPoint.y, startPoint.x);
		float stopAngle = atan2f(stopPoint.y, stopPoint.x);
		//ofLine(0, 0, cos(startAngle) * 1000, sin(startAngle) * 1000);
		//ofLine(0, 0, cos(stopAngle) * 1000, sin(stopAngle) * 1000);
		
		ofPolyline partial;
		for(int i = 0; i < polyline.size(); i++) {
			ofVec2f cur = polyline[i];
			cur -= center;
			cur.rotate(-ellipse.angle);
			float a = ellipse.size.width / 2, b = ellipse.size.height / 2, x0 = cur.x, y0 = cur.y;
			float scale = (a * b) / sqrtf(a * a * y0 * y0 + b * b * x0 * x0);
			partial.addVertex(cur.x * scale, cur.y * scale);
		}
		partial.draw();
	}
	ofPopMatrix();
	
	ofSetColor(255, 16);
	triangle.draw();
	quad.draw();
}

void testApp::updateGestures() {
	if(polyline.size() > 5) {
		ellipse = fitEllipse(polyline);
	}
	fitLine(polyline, linePoint, lineDirection);
	rect = minAreaRect(polyline);
	
	vector<cv::Point2f> points = toCv(polyline);
	triangle = toOf(getConvexPolygon(points, 3));
	quad = toOf(getConvexPolygon(points, 4));
}

void testApp::mousePressed(int x, int y, int button) {
	polyline.clear();
}

void testApp::mouseDragged(int x, int y, int button) {
	polyline.addVertex(x, y);
	updateGestures();
}
