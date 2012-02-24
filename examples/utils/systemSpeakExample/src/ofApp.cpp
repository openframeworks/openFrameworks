#include "ofApp.h"

static string voices[24] = {"Agnes", "Albert", "Alex", "Bad News", "Bahh", "Bells", "Boing", "Bruce", "Bubbles", "Cellos", "Deranged", "Fred", "Good News", "Hysterical", "Junior", "Kathy", "Pipe Organ", "Princess", "Ralph", "Trinoids", "Vicki", "Victoria", "Whisper", "Zarvox"};

//--------------------------------------------------------------
void ofApp::setup() {

    font.loadFont("verdana.ttf", 34);
    voice        = "Cellos";                  
    bRandomVoice = false;
    
    // load the lyrics from a text file and split them
    // up in to a vector of strings
    string lyrics = ofBufferFromFile("lyrics.txt").getText();
    step  = 0;
    words = ofSplitString(lyrics, " ");
}


//--------------------------------------------------------------
void ofApp::update() {
	
     // get a random voice
    if(bRandomVoice) {
        voice = voices[(int)ofRandom(24)];                  
    }
}


//--------------------------------------------------------------
void ofApp::draw() {
  
    
 
    // center the word on the screen
    float x = (ofGetWidth() - font.stringWidth(words[step])) / 2;
    float y = ofGetHeight() / 2;
    
    // call the system command say
    float rate = 400;
    string cmd = "say -v "+voice+" -r "+ofToString(rate)+" "+words[step]+" ";   // create the command
    system(cmd.c_str());
    
    // draw the word
    ofSetColor(0);
    font.drawString(words[step], x, y);
        
    // step to the next word
    step ++;
    step %= words.size();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if(key == 'r') {
        bRandomVoice = !bRandomVoice;
    }
    
    if(key == ' ') {
        voice = voices[(int)ofRandom(24)];                  
    }
}