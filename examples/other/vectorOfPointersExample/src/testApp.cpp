#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(15);
    ofEnableSmoothing();
    
    bArrowToPrev = false;
    time = 0;
    blink = 0;
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
        
        ofSetColor( abs(sin(ofMap(blink, 0, chain.size(), 0, PI)))*255);
        
        // DRAW ARROWS in the direction if it was a pointer to that direction
        if (bArrowToPrev){
            if ( chain[i]->prev != NULL ){
                drawArrow(chain[i]->pos, chain[i]->prev->pos, 3);
                blink = (i + time)%chain.size();
            }
        } else {
            if ( chain[i]->next != NULL ){
                drawArrow(chain[i]->pos, chain[i]->next->pos, 3);
                blink = (chain.size()-i - time )% chain.size();
            }
        }
        
    }
    
    ofSetColor(255);
    
    if (bArrowToPrev){
        time++;
        ofDrawBitmapString("Pointing to the previus", 15,15);
    } else {
        time--;
        ofDrawBitmapString("Pointing to the next", 15,15);
    }
    
    ofDrawBitmapString("Press 'd' to delet the last one, 'c' for all", 15,30);
    ofDrawBitmapString("and any other key to change the direction", 15,45);
}

void testApp::createNew(int _x, int _y){
    // For defining a pointer you have to use the command "new"
    Link * newLink = new Link(_x,_y); 
    
    // It takes the pointer to the last Link and make it store it as a PREVius one
    newLink->prev = last;               
    
    // This Link becomes the next of the last one;
    if (chain.size() > 0)
        chain.back()->next = newLink;   
    
    // then is pushed into the vector
    chain.push_back(newLink);           
    
    // Now on previus it«s a pointer to the newLink
    last = newLink;
    time--;
}

void testApp::deleteLast(){
    if (chain.size() > 0){
        // First you need to delete pointer from memory with "delete" command
        delete chain.back();       
    
        // Next delete the object from the vector 
        chain.erase( chain.end()-1 );   

        // This is just for clear the dependences
        if ( chain.size() > 1)
            chain[ chain.size() - 1 ]->next = NULL; 
        
        time++;
    }
}

void testApp::deleteAll(){
    for (int i = 0; i < chain.size(); i++){
        delete chain.back();
    }
    
    chain.clear();
}

void testApp::drawArrow(ofPoint from, ofPoint to, float size){
    ofVec2f arrowPoint = to - from;
    
    arrowPoint.normalize();
    float radial = atan2(arrowPoint.y,arrowPoint.x) + PI/2;
    arrowPoint *= 15;
    arrowPoint += from;
    
    ofLine(from, arrowPoint);
    
    ofPushMatrix();
    ofTranslate( arrowPoint );
    ofFill();
    ofRotate( ofRadToDeg(radial) );
    ofBeginShape();
    ofVertex(0, -size*2);
    ofVertex(-size, size*2);
    ofVertex(size, size*2);
    ofEndShape();
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key) {
		case 'd':
            deleteLast();
            break;
        case 'c':
            deleteAll();
            break;
        default:
            bArrowToPrev = !bArrowToPrev;
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
    createNew(x, y);
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