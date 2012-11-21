#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
// ofSetVerticalSync(true);
        // load an image from disk
        img.loadImage("linzer.png");
        // we're going to load a ton of points into an ofMesh
        mesh.setMode(OF_PRIMITIVE_POINTS);
        // loop through the image in the x and y axes
        int skip = 3; // load a subset of the points
        for(int y = 0; y < img.getHeight(); y += skip) {
                for(int x = 0; x < img.getWidth(); x += skip) {
                        ofColor cur = img.getColor(x, y);
                        if(cur.a > 0) {
                                // the alpha value encodes depth, let's remap it to a good depth range
                                float z = ofMap(cur.a, 0, 255, -300, 300);
                                cur.a = 255;
                                mesh.addColor(cur);
                                ofVec3f pos(x, y, z);
                                mesh.addVertex(pos);
                        }
                }
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
        glPointSize(2); // make the points bigger
	count = 0;
}

//--------------------------------------------------------------
void testApp::exit(){}

//--------------------------------------------------------------
void testApp::update(){}

//--------------------------------------------------------------
void testApp::draw() {
//        ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);
	count++;
	count = count%60;
	float r = ofMap(count, 0, 60, 0, 360);
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	ofScale(.5, 1, 1); // flip the y axis and zoom in a bit
        //ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2);
	ofRotateY(r);
	mesh.draw();
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){}

//--------------------------------------------------------------
void testApp::keyReleased(int key){}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}
