#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
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
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw() {
    
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
        if(letterCount < seussLines[lineCount].size()) {
           
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
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
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