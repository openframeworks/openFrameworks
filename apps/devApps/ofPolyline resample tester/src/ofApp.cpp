#include "ofApp.h"

ofPolyline poly;
unsigned int nearestIndex = 0;
float rotAngle = 0;
bool bRotate = false;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if(bRotate) rotAngle = ofGetElapsedTimef() * 10;
}

//--------------------------------------------------------------
ofPoint getPoint(float x, float y) {
    ofPoint p(x - ofGetWidth()/2, y - ofGetHeight()/2, 0);
    //    p.z = 300 * ofSignedNoise(x / ofGetWidth(), y / ofGetHeight(), ofGetElapsedTimef());
	p.rotate(-rotAngle, ofVec3f(0, 1, 0));
    return p;
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(poly.size() < 2) return;
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofRotateY(rotAngle);
    
    ofSetColor(255, 255, 255);
    poly.draw();
    
    ofSetColor(0, 255, 0);
    ofSetRectMode(OF_RECTMODE_CENTER);
    glPointSize(5);
    glBegin(GL_POINTS);
    for(int i=0; i<poly.size(); i++) {
        ofPoint p = poly[i];
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    
    for(int i=0; i<poly.size(); i++) {
        ofPoint p = poly[i];
        ofSetColor(255, 0, 0);
        ofLine(p, p + poly.getTangentAtIndex(i) * 20);
        
        ofSetColor(0, 255, 0);
        ofLine(p, p + poly.getNormalAtIndex(i) * 20);
        
        ofSetColor(0, 128, 255);
        ofLine(p, p + poly.getRotationAtIndex(i) * 20);
    }
    
    
    
    float totalLength = poly.getPerimeter();
    float totalArea = poly.getArea();
    ofPoint nearestPoint = poly.getClosestPoint(ofPoint(mouseX-ofGetWidth()/2, mouseY-ofGetHeight()/2), &nearestIndex);
    ofPoint nearestDataPoint = poly[nearestIndex];
    float lengthAtIndex = poly.getLengthAtIndex(nearestIndex);
    ofPoint pointAtIndex = poly.getPointAtIndexInterpolated(nearestIndex);
    ofPoint pointAtLength = poly.getPointAtLength(lengthAtIndex);
    ofPoint pointAtPercent = poly.getPointAtPercent(lengthAtIndex / totalLength);
    float indexAtLength = poly.getIndexAtLength(lengthAtIndex);
    
    float sinTime = ofMap(sin(ofGetElapsedTimef() * 0.5), -1, 1, 0, 1);
    float sinIndex = sinTime * (poly.isClosed() ? poly.size() : (poly.size()-1));  // sinTime mapped to indices direct
    float sinIndexLength = poly.getIndexAtPercent(sinTime); // sinTime mapped to indices based on length
    
    float lengthAtIndexSin = poly.getLengthAtIndexInterpolated(sinIndex);
    ofPoint pointAtIndexSin = poly.getPointAtIndexInterpolated(sinIndex);
    ofPoint pointAtPercentSin = poly.getPointAtPercent(sinTime);
    
    float angleAtIndex = poly.getAngleAtIndex(nearestIndex);
    float angleAtIndexSin = poly.getAngleAtIndexInterpolated(sinIndex);
    
    ofVec3f rotAtIndex = poly.getRotationAtIndex(nearestIndex);
    ofVec3f rotAtIndexSin = poly.getRotationAtIndexInterpolated(sinIndex);
    
    float rotMagAtIndex = rotAtIndex.length();
    float rotMagAtIndexSin = rotAtIndexSin.length();
    
    ofVec3f normalAtIndex = poly.getNormalAtIndex(nearestIndex);
    
    ofVec3f tangentAtIndexSin = poly.getTangentAtIndexInterpolated(sinIndex);
    ofVec3f normalAtIndexSin = poly.getNormalAtIndexInterpolated(sinIndex);
    ofVec3f rotationAtIndexSin = poly.getRotationAtIndexInterpolated(sinIndex);
    
    
    ofNoFill();
    ofSetLineWidth(2);
    
    ofSetColor(255, 0, 0);
    ofCircle(nearestPoint, 5);
    
    ofSetColor(255, 255, 0);
    ofCircle(nearestDataPoint, 7);
    
    // interpolating on indices
    {
        ofPoint p = poly.getPointAtIndexInterpolated(sinIndex);

        ofSetColor(0, 255, 255);
        ofCircle(p, 10);
        
        ofSetColor(255, 0, 0);
        ofLine(p, p + poly.getTangentAtIndexInterpolated(sinIndex) * 60);
        
        ofSetColor(0, 255, 0);
        ofLine(p, p + poly.getNormalAtIndexInterpolated(sinIndex) * 60);
        
        ofSetColor(0, 128, 255);
        ofLine(p, p + poly.getRotationAtIndexInterpolated(sinIndex) * 60);
    }
    
    // interpolating on length percentages
    {
        ofPoint p = poly.getPointAtIndexInterpolated(sinIndexLength);
        
        ofSetColor(255, 0, 255);
        ofCircle(p, 10);
        
        ofSetColor(255, 0, 0);
        ofLine(p, p + poly.getTangentAtIndexInterpolated(sinIndexLength) * 60);
        
        ofSetColor(0, 255, 0);
        ofLine(p, p + poly.getNormalAtIndexInterpolated(sinIndexLength) * 60);
        
        ofSetColor(0, 128, 255);
        ofLine(p, p + poly.getRotationAtIndexInterpolated(sinIndexLength) * 60);
    }
    
    
    ofSetColor(255, 255, 255);
    ofCircle(poly.getCentroid2D(), 20);
    
    ofPopMatrix();
    
    
    stringstream s;
    s << "Number of points: " << poly.size() << endl;
    s << "totalLength: " << totalLength << endl;
    
    s << endl;
    s << "nearestIndex: " << nearestIndex << endl;
    s << "nearestPoint: " << nearestPoint << endl;
    s << "nearestDataPoint: " << nearestDataPoint << endl;
    
    s << endl;
    s << "lengthAtIndex: " << lengthAtIndex << endl;
    s << "pointAtIndex: " << pointAtIndex << endl;
    
    s << endl;
    s << "pointAtLength: " << pointAtLength << endl;
    s << "pointAtPercent: " << pointAtPercent << endl;
    
    s << endl;
    s << "indexAtLength: " << indexAtLength << endl;
    
    
    s << endl;
    s << "sinTime: " << sinTime << endl;
    s << "sinIndex: " << sinIndex << endl;
    s << "sinIndexLength: " << sinIndexLength << endl;
    
    s << endl;
    s << "lengthAtIndexSin: " << lengthAtIndexSin << endl;
    s << "pointAtIndexSin: " << pointAtIndexSin << endl;
    s << "pointAtPercentSin: " << pointAtPercentSin << endl;
    
    s << endl;
    s << "angleAtIndex: " << angleAtIndex << endl;
    s << "angleAtIndexSin: " << angleAtIndexSin << endl;
    
    s << endl;
    s << "rotAtIndex: " << rotAtIndex << endl;
    s << "rotAtIndexSin: " << rotAtIndexSin << endl;
    
    s << endl;
    s << "rotMagAtIndex: " << rotMagAtIndex << endl;
    s << "rotMagAtIndexSin: " << rotMagAtIndexSin << endl;
    
    s << endl;
    s << "normalAtIndex: " << normalAtIndex << endl;
    s << "normalAtIndexSin: " << normalAtIndexSin << endl;
    
    ofSetColor(255);
    ofDrawBitmapString(s.str(), 10, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 'R': bRotate ^= true; if(!bRotate) rotAngle = 0; break;
        case 'r': poly.clear(); break;
        case 'c': poly.curveTo(mouseX, mouseY); break;
        case 'a': poly.arc(mouseX, mouseY, 50, 50, 0, 180); break;
        case 'o': poly.setClosed(!poly.isClosed()); break;
            
        case 'F': poly.simplify(10); break;
        case 'M': poly = poly.getSmoothed(5); break;
        case 'S': poly = poly.getResampledBySpacing(30); break;
        case 'C': poly = poly.getResampledByCount(50); break;
            
        case 'l': poly.setClosed(!poly.isClosed());
            
        case 'i': poly.insertVertex(ofPoint(mouseX, mouseY, 0), nearestIndex); break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    poly.lineTo(getPoint(x, y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    poly.lineTo(getPoint(x, y));
}
