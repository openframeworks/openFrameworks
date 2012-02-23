#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(15);
    ofEnableSmoothing();
    
    bDrawArrows = true;
    last = NULL;    // This is the end of the chain
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    for (int i = 0; i < chain.size(); i++){
        // Draw every Link
        ofSetColor(255);
        ofNoFill();
        ofCircle( chain[i]->pos, 5);
        
        // if it have a previus Link draw a line to it
        if ( chain[i]->previus != NULL ){
            ofVec2f arrowPoint = chain[i]->pos - chain[i]->previus->pos;
            
            arrowPoint.normalize();
            float radial = atan2(arrowPoint.y,arrowPoint.x) + PI/2;
            arrowPoint *= 15;
            arrowPoint += chain[i]->pos;
            
            ofLine(chain[i]->pos, arrowPoint);
            
            if (bDrawArrows){
                ofPushMatrix();
                ofTranslate(arrowPoint);
                ofSetColor(170);
                ofFill();
                ofRotate( ofRadToDeg(radial) );
                ofBeginShape();
                ofVertex(0, -6);
                ofVertex(-3, 6);
                ofVertex(3, 6);
                ofEndShape();
                ofPopMatrix();
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    bDrawArrows = !bDrawArrows;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    Link * newLink = new Link(x,y);
    
    newLink->previus = last;
    
    last = newLink;     // Now on previus it«s a pointer to the newLink
    chain.push_back(newLink);   // New link it«s added to 
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    last = NULL;    // This cut the chain
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