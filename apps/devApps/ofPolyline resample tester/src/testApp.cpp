#include "testApp.h"

ofPolyline poly;

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    
}

//--------------------------------------------------------------
void testApp::update(){
    
}


//--------------------------------------------------------------
void testApp::draw(){
    if(poly.size() < 2) return;
    
    ofSetColor(255, 255, 255);
    poly.draw();
    
    ofSetColor(0, 255, 0);
    glPointSize(5);
    for(int i=0; i<poly.size(); i++) {
        glBegin(GL_POINTS);
        for(int i=0; i<poly.size(); i++) {
            ofPoint &p = poly[i];
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }
    
    
    float totalLength = poly.getPerimeter();;
    unsigned int nearestIndex;
    ofPoint nearestPoint = poly.getClosestPoint(ofPoint(mouseX,mouseY), &nearestIndex);
    ofPoint nearestDataPoint = poly[nearestIndex];
    float lengthAtIndex = poly.getLengthAtIndex(nearestIndex);
//    float lengthAtNormalisedIndex = poly.getLengthAtNormalisedIndex(nearestIndex / (float)poly.size());
    ofPoint pointAtIndex = poly.getPointAtIndexInterpolated(nearestIndex);
//    ofPoint pointAtNormalisedIndex = poly.getPointAtNormalisedIndex(nearestIndex / (float)poly.size());
    ofPoint pointAtLength = poly.getPointAtLength(lengthAtIndex);
    ofPoint pointAtNormalisedLength = poly.getPointAtNormalisedLength(lengthAtIndex / totalLength);
    float indexAtLength = poly.getIndexAtLength(lengthAtIndex);
    
    float t = ofMap(sin(ofGetElapsedTimef() * 0.5), -1, 1, 0, 1);
    float i = t * (poly.size()-1);
    float lengthAtIndexSin = poly.getLengthAtIndexInterpolated(i);
    ofPoint pointAtIndexSin = poly.getPointAtIndexInterpolated(i);
    ofPoint pointAtNormalisedLengthSin = poly.getPointAtNormalisedLength(t);
    
    float curvatureAtIndex = poly.getCurvatureAtIndex(nearestIndex);
    float curvatureAtIndexSin = poly.getCurvatureAtIndexInterpolated(i);
    
    ofNoFill();
    ofSetLineWidth(2);
    
    ofSetColor(255, 0, 0);
    ofCircle(nearestPoint, 5);
    
    ofSetColor(255, 255, 0);
    ofCircle(nearestDataPoint, 7);
    
    ofSetColor(0, 0, 255);
    ofCircle(pointAtIndexSin, 10);
    
    ofSetColor(255, 0, 255);
    ofCircle(pointAtNormalisedLengthSin, 15);
    
    
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
    s << "pointAtNormalisedLength: " << pointAtNormalisedLength << endl;
    
    s << endl;
    s << "indexAtLength: " << indexAtLength << endl;
    
    
    s << endl;
    s << "t: " << t << endl;
    s << "i: " << i << endl;
    
    s << endl;
    s << "lengthAtIndexSin: " << lengthAtIndexSin << endl;
    s << "pointAtIndexSin: " << pointAtIndexSin << endl;
    s << "pointAtNormalisedLengthSin: " << pointAtNormalisedLengthSin << endl;
    
    s << endl;
    s << "curvatureAtIndex: " << curvatureAtIndex << endl;
    s << "curvatureAtIndexSin: " << curvatureAtIndexSin << endl;
    
    
    ofSetColor(255);
    ofDrawBitmapString(s.str(), 10, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key) {
        case 'r': poly.clear(); break;
        case 'c': poly.curveTo(mouseX, mouseY); break;
        case 'a': poly.arc(mouseX, mouseY, 50, 50, 0, 180); break;
        case 'o': poly.setClosed(!poly.isClosed()); break;
            
        case 'S': poly = poly.getResampledBySpacing(30); break;
        case 'C': poly = poly.getResampledByCount(50); break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    poly.lineTo(x, y);
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    poly.lineTo(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}