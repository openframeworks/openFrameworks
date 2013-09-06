#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofBackgroundHex(0xff72a1);
    ofSetFrameRate(30);
    
    nextLetterTime = ofGetElapsedTimeMillis();
    lineCount      = 0;
    letterCount    = 0;
    
    // this is our buffer to stroe the text data
    ofBuffer buffer = ofBufferFromFile("ohplaces.txt");
    
    if(buffer.size()) { 
                
        // we now keep grabbing the next line
        // until we reach the end of the file
        while(buffer.isLastLine() == false) {
            
            // move on to the next line
            string line = buffer.getNextLine();
            
            // copy the line to draw later
            // make sure its not a empty line
            if(line.empty() == false) {
                seussLines.push_back(line);
            }
            
            // print out the line
            cout << line << endl;
            
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    // the total width on the line
    int strWidth = (seussLines[lineCount].length()*8) + 5;
    
    // x and y for the drawing
    float x = (ofGetWidth()-strWidth)/2;
    float y = ofGetHeight()/2;
    
    
    // we are slowy grabbing part of the line
    string typedLine = seussLines[lineCount].substr(0, letterCount);
    
    // draw the line
    ofSetColor(0);
    ofRect(x, y, strWidth, 15);
    ofSetColor(255);
    ofDrawBitmapString(typedLine, x+4, y+11);
    
    
    // this is our timer for grabbing the next letter
    float time = ofGetElapsedTimeMillis() - nextLetterTime;
    
    if(time > 9) {
        
        // increment the letter count until 
        // we reach the end of the line
        if(letterCount < (int)seussLines[lineCount].size()) {
           
            // move on to the next letter
            letterCount ++;
            
            // store time for next letter type
            nextLetterTime = ofGetElapsedTimeMillis();
            
        }
        else {
            
            // wait just a flash then move on 
            // to the next line...
            if(time > 300) {
                
                nextLetterTime = ofGetElapsedTimeMillis();
                letterCount = 0;
                lineCount ++;
                lineCount %= seussLines.size();
            }
            
        }
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
