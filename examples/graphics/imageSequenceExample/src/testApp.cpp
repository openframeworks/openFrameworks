#include "testApp.h"


/*
 
 Image Sequence Example:
 In this example we are going to load a sequence of images from a folder.
 We know that the images are in order and the we saved them out at 24fps. 
 the goal is to play them back at 24fps independent of the fps of the app.
 You can toggle the sequence to be independent of the app fps.
 
 Topics:
 - ofDirectory
 - ofImage
 - timing
 
 gif from: http://probertson.livejournal.com/32350.html
 
 */


//--------------------------------------------------------------
void testApp::setup() {
    
    ofBackground(0);
    ofSetWindowTitle("The Moon is made of plops");
                     
    // read the directory for the images
    // we know that they are named in seq
    ofDirectory dir;
    
    int nFiles = dir.listDir("plops");
    if(nFiles) {
        
        for(int i=0; i<dir.numFiles(); i++) { 
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            images.push_back(ofImage());
            images.back().loadImage(filePath);
            
        }
        
    } 
    else printf("Could not find folder\n");
    
    // this toggle will tell the sequence
    // be be indepent of the app fps
    bFrameIndependent = true;
    
    // this will set the speed to play 
    // the animation back we set the
    // default to 24fps
    sequenceFPS = 24;

    // set the app fps 
    appFPS = 60;
    ofSetFrameRate(appFPS);
    
}

//--------------------------------------------------------------
void testApp::update() {
    
}

//--------------------------------------------------------------
void testApp::draw() {
    
    // we need some images if not return
    if((int)images.size() <= 0) {
        ofSetColor(255);
        ofDrawBitmapString("No Images...", 150, ofGetHeight()/2);
        return;
    }
    
    // this is the total time of the animation based on fps
    //float totalTime = images.size() / sequenceFPS;
    
    
    int frameIndex = 0;
    
    if(bFrameIndependent) {
        // calculate the frame index based on the app time
        // and the desired sequence fps. then mod to wrap
        frameIndex = (int)(ofGetElapsedTimef() * sequenceFPS) % images.size();
    }
    else {
        // set the frame index based on the app frame
        // count. then mod to wrap.
        frameIndex = ofGetFrameNum() % images.size();
    }
    
    // draw the image sequence at the new frame count
    ofSetColor(255);
    images[frameIndex].draw(256, 36);
    
    
    // draw where we are in the sequence
    float x = 0;
    for(int offset = 0; offset < 5; offset++) {
			int i = (frameIndex + offset) % images.size();
        ofSetColor(255);
        images[i].draw(200+x, ofGetHeight()-40, 40, 40);
        x += 40;
    }
    
    
    // how fast is the app running and some other info
    ofSetColor(50);
    ofRect(0, 0, 200, 200);
    ofSetColor(200);
    string info;
    info += ofToString(frameIndex)+" sequence index\n";
    info += ofToString(appFPS)+"/"+ofToString(ofGetFrameRate(), 0)+" fps\n";
    info += ofToString(sequenceFPS)+" Sequence fps\n\n";
    info += "Keys:\nup & down arrows to\nchange app fps\n\n";
    info += "left & right arrows to\nchange sequence fps";
    info += "\n\n't' to toggle\nframe independent("+ofToString(bFrameIndependent)+")";
    
    ofDrawBitmapString(info, 15, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    
    if(key == OF_KEY_UP)      appFPS ++;
    if(key == OF_KEY_DOWN)    appFPS --;
    if(key == OF_KEY_LEFT)    sequenceFPS --;
    if(key == OF_KEY_RIGHT)   sequenceFPS ++;    
    if(key == 't')            bFrameIndependent = !bFrameIndependent;
    
    // check for less than zero...
    sequenceFPS = MAX(sequenceFPS, 1);
    appFPS      = MAX(appFPS, 1);
    
    ofSetFrameRate(appFPS);
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
