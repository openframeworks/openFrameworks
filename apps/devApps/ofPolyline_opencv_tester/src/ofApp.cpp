#include "ofApp.h"
#include "ofxOpenCv.h"

ofPolyline poly;
ofPolyline cur;

int camWidth = 640;
int camHeight = 480;
ofxCvColorImage cvImgColor;
ofxCvGrayscaleImage cvImgGrayscale;
ofxCvContourFinder contourFinder;
ofImage img;
float rotAngle = 0;

void ofApp::setup(){
    camWidth = 640;
    camHeight = 480;
    cvImgColor.allocate(camWidth, camHeight);
    
    cvImgGrayscale.allocate(camWidth, camHeight);
    ofBackground(0);
    img.loadImage("testimage.png");
    img.setImageType(OF_IMAGE_COLOR);
}

void ofApp::update(){
    
    cvImgColor.setFromPixels(img.getPixelsRef());
    cvImgGrayscale.setFromColorImage(cvImgColor);
    cvImgGrayscale.threshold(50, true);
    int num = contourFinder.findContours(cvImgGrayscale, 64 * 64, camWidth * camHeight, 5, false, true);
    if(num>=1){
        cur.clear();
        cur.addVertices(contourFinder.blobs[0].pts);
        cur.close();
        cur.simplify(0.5);
        poly.clear();
        poly = cur.getResampledBySpacing(20);
    }
}

void ofApp::draw(){
    
    ofSetColor(255);
    ofSetRectMode(OF_RECTMODE_CORNER);
    cvImgGrayscale.draw(0, 0);
    
    
    if(poly.size() < 2) return;
    
    ofPushMatrix();
    ofTranslate(0,0);
    ofRotateY(rotAngle);
    
    ofSetColor(255, 255, 255);
    poly.draw();
    
    ofSetColor(0, 255, 0);
    glPointSize(5);
    glBegin(GL_POINTS);
    for(int i=0; i<poly.size(); i++) {
        ofPoint p = poly[i];
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    
    for(int i=0; i<poly.size(); i++) {
        ofPoint p = poly[i];
        ofSetColor(0, 255, 0);
        ofLine(p, p + poly.getNormalAtIndex(i) * 20);
    }
}
