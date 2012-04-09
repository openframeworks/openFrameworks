#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofBackgroundHex(0xc5c9b2);
    
    bTimerReached = false;
    startTime = ofGetElapsedTimeMillis();  // get the start time
    endTime = (int)ofRandom(1000, 5000); // in milliseconds
}

//--------------------------------------------------------------
void testApp::update() {
    
}
//--------------------------------------------------------------
void testApp::draw(){

    float barWidth = 500;
		
    // update the timer this frame
    float timer = ofGetElapsedTimeMillis() - startTime;
    
    if(timer >= endTime && !bTimerReached) {
        bTimerReached = true;        
        ofMessage msg("Timer Reached");
        ofSendMessage(msg);
    }
    
    // the background to the progress bar
    ofSetColor(100);
    ofRect((ofGetWidth()-barWidth)/2, ofGetHeight()/2, barWidth, 30);
    
    // get the percantage of the timer
    float pct = ofMap(timer, 0.0, endTime, 0.0, 1.0, true);
    ofSetHexColor(0xf02589);
    ofRect((ofGetWidth()-barWidth)/2, ofGetHeight()/2, barWidth*pct, 30);

    // draw the percantage
    ofSetColor(20);
    ofDrawBitmapString(ofToString(pct*100, 0)+"%", ((ofGetWidth()-barWidth)/2)+barWidth+10, (ofGetHeight()/2)+20);
    
    // the timer was reached :)
    if(bTimerReached) {
        ofSetColor(20);
        ofDrawBitmapString("Timer Reached!", (ofGetWidth()-100)/2, (ofGetHeight()/2)+70);
    }
    
    // some information about the timer
    string  info  = "FPS:        "+ofToString(ofGetFrameRate(),0)+"\n";
            info += "Start Time: "+ofToString(startTime, 1)+"\n";
            info += "End Time:   "+ofToString(endTime/1000.0, 1)+" seconds\n";
            info += "Timer:      "+ofToString(timer/1000.0, 1)+" seconds\n";
            info += "Percentage: "+ofToString(pct*100, 1)+"%\n";
            info += "\nPress ' ' to get a new random end time\n";
    ofSetColor(0);
    ofDrawBitmapString(info, 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    if(key == ' ') {
        bTimerReached = false;                     // reset the timer
        startTime = ofGetElapsedTimeMillis();  // get the start time
        endTime = (int)ofRandom(1000, 5000); // in milliseconds
    }
}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    startTime = ofGetElapsedTimeMillis();
    endTime = (int)ofRandom(100, 5000); // milliseconds
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    cout << "Got the message: " + msg.message << endl;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}